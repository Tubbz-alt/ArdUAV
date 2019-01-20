#include "GS_Tools.h"
#include "GS_Serial.h"

#include "Shared_Tools.h"
#include "AirComms.h"




/////////////////////////////////////////////////////////////////////////////////////////
//GS Class
GS_Class myGS;

void GS_Class::begin()
{
	//initialize serial streams	
	GS_DEBUG_PORT.begin(DEBUG_PORT_BAUD);
	GS_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
	GS_TELEM_PORT.begin(TELEM_PORT_BAUD);




	//wait for all serial ports to come online
	//while (!DEBUG_PORT);
	GS_DEBUG_PORT.println(F("Initializing serial ports..."));
	GS_DEBUG_PORT.print(F("Initializing command port at Serial")); GS_DEBUG_PORT.print(GS_COMMAND_PORT_NUMBER);  GS_DEBUG_PORT.println(F("..."));
	GS_DEBUG_PORT.print(F("Initializing telemetry at Serial")); GS_DEBUG_PORT.print(GS_TELEM_PORT_NUMBER);  GS_DEBUG_PORT.println(F("..."));
	while (!GS_COMMAND_PORT)
	{
		GS_DEBUG_PORT.println(F("Initializing command port..."));
		GS_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
		delay(500);
	}
	while (!GS_TELEM_PORT)
	{
		GS_DEBUG_PORT.println(F("Initializing telemetry port..."));
		GS_TELEM_PORT.begin(TELEM_PORT_BAUD);
		delay(500);
	}
	GS_DEBUG_PORT.println(F("\tAll ports sucessfully initialized"));




	//turn on PWR LED
	GS_DEBUG_PORT.println(F("Turning on PWR LED..."));
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	GS_DEBUG_PORT.println(F("\tPWR LED on"));




	//set analog to digital conversion resolution (in bits)
	GS_DEBUG_PORT.println(F("Setting ADC resolution (default - 16 bits)..."));
	analogReadResolution(ANALOG_RESOLUTION);
	GS_DEBUG_PORT.println(F("\tResolution set"));




	//initialize AirComs
	GS_DEBUG_PORT.println(F("Initializing AirComms..."));
	myRadio.begin(false);
	GS_DEBUG_PORT.println(F("\tAirComms initialized"));

	return;
}




int GS_Class::grabData_Radio()
{
	int result = myRadio.grabData_Radio();

	if (result == AIR_NEW_DATA)
	{
		telemetry.altitude = myRadio.incomingArray[AIR_ALTITUDE_INDEX] / 100.0;
		telemetry.rollAngle = myRadio.incomingArray[AIR_ROLL_ANGLE_INDEX] / 100.0;
		telemetry.pitchAngle = myRadio.incomingArray[AIR_PITCH_ANGLE_INDEX] / 100.0;
		telemetry.velocity = myRadio.incomingArray[AIR_PITOT_INDEX] / 100.0;
		telemetry.latitude = myRadio.incomingArray[AIR_LATITUDE_INDEX] / 100.0;
		telemetry.longitude = myRadio.incomingArray[AIR_LONGITUDE_INDEX] / 100.0;
	}

	return result;
}




//send telemetry data to GS
void GS_Class::sendCommands()
{
	//update the radio's outgoing array with the propper information
	myRadio.outgoingArray[AIR_PITCH_INDEX] = myGS.controlInputs.pitch_command;
	myRadio.outgoingArray[AIR_ROLL_INDEX] = myGS.controlInputs.roll_command;
	myRadio.outgoingArray[AIR_YAW_INDEX] = myGS.controlInputs.yaw_command;
	myRadio.outgoingArray[AIR_THROTTLE_INDEX] = myGS.controlInputs.throttle_command;
	/*myRadio.outgoingArray[AIR_AUTOPILOT_INDEX] = myGS.controlInputs.autopilot_command;
	myRadio.outgoingArray[AIR_LIMITER_INDEX] = myGS.controlInputs.limiter_command;
	myRadio.outgoingArray[AIR_LANDING_GEAR_INDEX] = myGS.controlInputs.landingGear_command;
	myRadio.outgoingArray[AIR_FLAPS_INDEX] = myGS.controlInputs.flaps_command;*/

	//send the telemetry data to GS
	myRadio.sendData();

	return;
}




//determine each command value based off GS sensor data
void GS_Class::computeCommands()
{
	//read and map joystick data
	//mapping normalized to max and min joystick values
	myGS.controlInputs.roll_command = updateServoCommand(ailerons);
	myGS.controlInputs.pitch_command = updateServoCommand(elevator);
	myGS.controlInputs.yaw_command = updateServoCommand(rudder);
	myGS.controlInputs.throttle_command = updateServoCommand(throttle);

	return;
}




int16_t GS_Class::updateServoCommand(controlSurfaces controlSurface)
{
	//value to be sent to plane
	uint16_t commandValue = 0;

	//read the analog voltage of the potentiometer
	uint16_t analogValue = analogRead(controlSurface.analog_pin);

	/*GS_DEBUG_PORT.print("Command_Index: "); GS_DEBUG_PORT.println(controlSurface.command_index);
	GS_DEBUG_PORT.print("ADC_min: "); GS_DEBUG_PORT.println(controlSurface.ADC_min);
	GS_DEBUG_PORT.print("Analog: "); GS_DEBUG_PORT.println(analogValue);
	GS_DEBUG_PORT.print("ADC_max: "); GS_DEBUG_PORT.println(controlSurface.ADC_max);
	GS_DEBUG_PORT.print("high_rates_surface_min: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_min);
	GS_DEBUG_PORT.println();*/

	//throttle is processed opposite of all other commands
	if (controlSurface.command_index == AIR_THROTTLE_INDEX)
	{
		//convert to a servo command format
		commandValue = map(analogValue,             //value to be mapped
			controlSurface.ADC_min,                 //input minimum expected value
			controlSurface.ADC_max,                 //input maximum expected value
			controlSurface.high_rates_surface_min,  //output minimum value
			controlSurface.high_rates_surface_max   //output maximum value
		) + controlSurface._offset;					//add the offset (bias)
	}
	else
	{
		//convert to a servo command format
		commandValue = map(analogValue,             //value to be mapped
			controlSurface.ADC_min,                 //input minimum expected value
			controlSurface.ADC_max,                 //input maximum expected value
			controlSurface.high_rates_surface_max,  //output maximum value
			controlSurface.high_rates_surface_min   //output minimum value
		) + controlSurface._offset;					//add the offset (bias)
	}

	commandValue = constrain(commandValue,          //value to be constrained
			controlSurface.high_rates_surface_min,	//minimum value
			controlSurface.high_rates_surface_max	//maximum value
		);

	/*GS_DEBUG_PORT.print("Command_Index: "); GS_DEBUG_PORT.println(controlSurface.command_index);
	GS_DEBUG_PORT.print("Value: "); GS_DEBUG_PORT.println(commandValue);
	GS_DEBUG_PORT.print("high_rates_surface_min: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_min);
	GS_DEBUG_PORT.print("high_rates_surface_max: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_max);
	GS_DEBUG_PORT.print("_offset: "); GS_DEBUG_PORT.println(controlSurface._offset);
	GS_DEBUG_PORT.println();*/

	return commandValue;
}
/////////////////////////////////////////////////////////////////////////////////////////