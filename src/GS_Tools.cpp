#include "GS_Tools.h"
#include "GS_Serial.h"
#include "Shared_Tools.h"
#include "SerialTransfer.h"




void GS_Class::begin()
{
	//initialize variables
	controlInputs.limiter_enable        = false;
	controlInputs.manual_control_enable = true;




	//initialize serial streams	
	GS_DEBUG_PORT.begin(DEBUG_PORT_BAUD);
	GS_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
	GS_TELEM_PORT.begin(TELEM_PORT_BAUD);




	//wait for all serial ports to come online
	//while (!GS_DEBUG_PORT);
	GS_DEBUG_PORT.println(F("Initializing serial ports..."));
	GS_DEBUG_PORT.print(F("Initializing command port at Serial")); GS_DEBUG_PORT.print(GS_COMMAND_PORT_NUMBER); GS_DEBUG_PORT.println(F("..."));
	while (!GS_COMMAND_PORT)
	{
		GS_DEBUG_PORT.println(F("Initializing command port..."));
		GS_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
		delay(500);
	}
	GS_DEBUG_PORT.print(F("Initializing telemetry at Serial")); GS_DEBUG_PORT.print(GS_TELEM_PORT_NUMBER); GS_DEBUG_PORT.println(F("..."));
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
	commandTransfer.begin(GS_COMMAND_PORT);
	telemetryTransfer.begin(GS_TELEM_PORT);
	GS_DEBUG_PORT.println(F("\tAirComms initialized"));




	//initialize "pass-through" timers
	GS_DEBUG_PORT.println(F("Initializing timers..."));
	commandTimer.begin(REPORT_COMMANDS_PERIOD);
	GS_DEBUG_PORT.println(F("\tTimers initialized..."));
	
	
	
	
	GS_DEBUG_PORT.println(F("Initialization complete"));
	GS_DEBUG_PORT.println(F("--------------------------------------------------"));
}




//determine each command value based off GS sensor data
void GS_Class::computeCommands()
{
	//read and map joystick data
	//mapping normalized to max and min joystick values
	controlInputs.roll_command     = updateServoCommand(ailerons);
	controlInputs.pitch_command    = updateServoCommand(elevator);
	controlInputs.yaw_command      = updateServoCommand(rudder);
	controlInputs.throttle_command = updateServoCommand(throttle);
}




//send telemetry data to GS
void GS_Class::sendCommands()
{
	//update the radio's outgoing array with the propper information
	commandTransfer.txObj(controlInputs);
	commandTransfer.sendData(sizeof(controlInputs) + COMMAND_BUFFER); //allow extra bytes to send (bytes are user defined)
}




//determine each command value based off GS sensor data and send commands to plane
void GS_Class::computeAndSendCommands()
{
	//use timer to send commands to the plane at a fixed rate
	if (commandTimer.fire())
	{
		computeCommands();
		sendCommands();
	}
}




int16_t GS_Class::updateServoCommand(const control_surfaces_struct& controlSurface)
{
	//value to be sent to plane
	uint16_t commandValue = 0;

	//read the analog voltage of the potentiometer
	uint16_t analogValue = analogRead(controlSurface.analog_pin);

	if (controlSurface.reverse)
	{
		//convert to a servo command format
		commandValue = map(analogValue,                //value to be mapped
		                   controlSurface.ADC_min,     //input minimum expected value
			               controlSurface.ADC_max,     //input maximum expected value
		                   controlSurface.surface_max, //output maximum value
		                   controlSurface.surface_min  //output minimum value
		               ) + controlSurface._offset;     //add the offset (bias)
	}
	else
	{
		//convert to a servo command format
		commandValue = map(analogValue,                //value to be mapped
			               controlSurface.ADC_min,     //input minimum expected value
			               controlSurface.ADC_max,     //input maximum expected value
			               controlSurface.surface_min, //output minimum value
			               controlSurface.surface_max  //output maximum value
		                   ) + controlSurface._offset; //add the offset (bias)
	}

	commandValue = constrain(commandValue,                //value to be constrained
		                     controlSurface.surface_min,  //minimum value
		                     controlSurface.surface_max); //maximum value

	return commandValue;
}




bool GS_Class::handleSerialEvents()
{
	if (telemetryTransfer.available())
	{
		uint16_t recLen = telemetryTransfer.rxObj(telemetry);
		telemetryTransfer.rxObj(controlInputs, recLen);

		lossLinkTimer.start();
		linkConnected = true;
	}

	return linkFailover();
}




bool GS_Class::linkFailover()
{
	if (lossLinkTimer.fire(false))
	{
		//do something

		linkConnected = false;
	}

	return linkConnected;
}
