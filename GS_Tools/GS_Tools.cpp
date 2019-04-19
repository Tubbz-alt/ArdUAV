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
	//while (!GS_DEBUG_PORT);
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




	//initialize "pass-through" timers
	GS_DEBUG_PORT.println(F("Initializing timers..."));
	timeBench_Commands = millis();
	currentTime_Commands = timeBench_Commands;

	timeBench_Telem = millis();
	currentTime_Telem = timeBench_Telem;
	GS_DEBUG_PORT.println(F("\tTimers initialized..."));

	return;
}




int GS_Class::grabData_Radio()
{
	int result;

	//check to see if there is a loss of radio link between GS and IFC
	checkRadioLink();
	
	//see if new data is available
	result = myRadio.grabData_Radio();

	//save new data if available
	if (result == AIR_NEW_DATA)
	{
		telemetry.altitude = myRadio.incomingArray[AIR_ALTITUDE_INDEX] / 100.0;
		telemetry.rollAngle = myRadio.incomingArray[AIR_ROLL_ANGLE_INDEX] / 100.0;
		telemetry.pitchAngle = myRadio.incomingArray[AIR_PITCH_ANGLE_INDEX] / 100.0;
		telemetry.velocity = myRadio.incomingArray[AIR_PITOT_INDEX] / 100.0;
		telemetry.latitude = myRadio.incomingArray[AIR_LATITUDE_INDEX] / 100.0;
		telemetry.longitude = myRadio.incomingArray[AIR_LONGITUDE_INDEX] / 100.0;
		telemetry.UTC_year = myRadio.incomingArray[AIR_UTC_YEAR_INDEX];
		telemetry.UTC_month = myRadio.incomingArray[AIR_UTC_MONTH_INDEX];
		telemetry.UTC_day = myRadio.incomingArray[AIR_UTC_DAY_INDEX];
		telemetry.UTC_hour = myRadio.incomingArray[AIR_UTC_HOUR_INDEX];
		telemetry.UTC_minute = myRadio.incomingArray[AIR_UTC_MINUTE_INDEX];
		telemetry.UTC_second = myRadio.incomingArray[AIR_UTC_SECOND_INDEX];
		telemetry.speedOverGround = myRadio.incomingArray[AIR_SOG_INDEX] / 100.0;
		telemetry.courseOverGround = myRadio.incomingArray[AIR_COG_INDEX] / 100.0;
	}

	//return boolean to tell main program if there is new data available
	return result;
}




//send data to datalogging computer via debugging port
void GS_Class::sendTelem()
{
	GS_DEBUG_PORT.println("New Telem:");
	GS_DEBUG_PORT.print("Alt: ");	GS_DEBUG_PORT.println(telemetry.altitude, 5);
	GS_DEBUG_PORT.print("Roll: ");	GS_DEBUG_PORT.println(telemetry.rollAngle, 5);
	GS_DEBUG_PORT.print("Pitch: ");	GS_DEBUG_PORT.println(telemetry.pitchAngle, 5);
	GS_DEBUG_PORT.print("Vel: ");	GS_DEBUG_PORT.println(telemetry.velocity, 5);
	GS_DEBUG_PORT.print("Lat: ");	GS_DEBUG_PORT.println(telemetry.latitude, 5);
	GS_DEBUG_PORT.print("Lon: ");	GS_DEBUG_PORT.println(telemetry.longitude, 5);
	GS_DEBUG_PORT.print("UTC_y: ");	GS_DEBUG_PORT.println(telemetry.UTC_year);
	GS_DEBUG_PORT.print("UTC_M: ");	GS_DEBUG_PORT.println(telemetry.UTC_month);
	GS_DEBUG_PORT.print("UTC_d: ");	GS_DEBUG_PORT.println(telemetry.UTC_day);
	GS_DEBUG_PORT.print("UTC_h: ");	GS_DEBUG_PORT.println(telemetry.UTC_hour);
	GS_DEBUG_PORT.print("UTC_m: ");	GS_DEBUG_PORT.println(telemetry.UTC_minute);
	GS_DEBUG_PORT.print("UTC_s: ");	GS_DEBUG_PORT.println(telemetry.UTC_second);
	GS_DEBUG_PORT.print("SOG: ");	GS_DEBUG_PORT.println(telemetry.speedOverGround, 3);
	GS_DEBUG_PORT.print("COG: ");	GS_DEBUG_PORT.println(telemetry.courseOverGround, 3);
	GS_DEBUG_PORT.println();

	return;
}




//determine each command value based off GS sensor data
void GS_Class::computeCommands()
{
	//read and map joystick data
	//mapping normalized to max and min joystick values
	controlInputs.roll_command = updateServoCommand(ailerons);
	controlInputs.pitch_command = updateServoCommand(elevator);
	controlInputs.yaw_command = updateServoCommand(rudder);
	controlInputs.throttle_command = updateServoCommand(throttle);

	return;
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

	//optional debugging prints to see what is in the outgoing array
	/*for (byte i = 0; i < AIR_DATA_LEN; i++)
	{
		Serial.println(myRadio.outgoingArray[i]);
	}
	Serial.println();*/

	//send the telemetry data to GS
	myRadio.sendData();

	return;
}




//determine each command value based off GS sensor data and send commands to plane
void GS_Class::computeAndSendCommands()
{
	//use timer to send commands to the plane at a fixed rate
	currentTime_Commands = millis();
	if ((currentTime_Commands - timeBench_Commands) >= REPORT_COMMANDS_PERIOD)
	{
		//reset timer
		timeBench_Commands = currentTime_Commands;

		//determine each command value based off GS sensor data
		computeCommands();

		//send commands to plane
		sendCommands();
	}

	return;
}




//check to see if there is a loss of radio link between GS and IFC
bool GS_Class::checkRadioLink()
{
	//see if new data has arrived - if not, test if the timeout condition is met
	if (!myRadio.arrayComplete_Radio)
	{
		//timer to see if there is a loss of link between GS and plane
		myGS.currentTime_Telem = millis();

		if (!noPacketFlag)
		{
			myGS.timeBench_Telem = myGS.currentTime_Telem;
			noPacketFlag = true;
		}

		if ((myGS.currentTime_Telem - myGS.timeBench_Telem) >= LOSS_LINK_TIMEOUT)
		{
			//link is severed - unset flag if not alread unset
			linkConnected = false;
			GS_DEBUG_PORT.println("loss link");
		}
		else
		{
			//not enough time has passed to determine if the link is severed - set flag if not already set
			//do not reset timer
			linkConnected = true;
		}
	}
	else
	{
		//new data has arrived - link is connected
		linkConnected = true;
		noPacketFlag = false;
	}

	return linkConnected;
}




int16_t GS_Class::updateServoCommand(controlSurfaces controlSurface)
{
	//value to be sent to plane
	uint16_t commandValue = 0;

	//read the analog voltage of the potentiometer
	uint16_t analogValue = analogRead(controlSurface.analog_pin);

	//optional debugging prints
	/*if (controlSurface.command_index == AILERON_INDEX)
	{
		GS_DEBUG_PORT.print("Command_Index: "); GS_DEBUG_PORT.println(controlSurface.command_index);
		GS_DEBUG_PORT.print("ADC_min: "); GS_DEBUG_PORT.println(controlSurface.ADC_min);
		GS_DEBUG_PORT.print("Analog: "); GS_DEBUG_PORT.println(analogValue);
		GS_DEBUG_PORT.print("ADC_max: "); GS_DEBUG_PORT.println(controlSurface.ADC_max);
		GS_DEBUG_PORT.print("high_rates_surface_min: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_min);
		GS_DEBUG_PORT.println();
	}*/
	

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

	//optional debugging prints
	/*if (controlSurface.command_index == AILERON_INDEX)
	{
		GS_DEBUG_PORT.print("Command_Index: "); GS_DEBUG_PORT.println(controlSurface.command_index);
		GS_DEBUG_PORT.print("Value: "); GS_DEBUG_PORT.println(commandValue);
		GS_DEBUG_PORT.print("high_rates_surface_min: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_min);
		GS_DEBUG_PORT.print("high_rates_surface_max: "); GS_DEBUG_PORT.println(controlSurface.high_rates_surface_max);
		GS_DEBUG_PORT.print("_offset: "); GS_DEBUG_PORT.println(controlSurface._offset);
		GS_DEBUG_PORT.println();
	}*/

	return commandValue;
}
/////////////////////////////////////////////////////////////////////////////////////////
