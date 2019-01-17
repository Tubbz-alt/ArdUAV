#include "GS_Tools.h"

#include "Shared_Tools.h"




/////////////////////////////////////////////////////////////////////////////////////////
//GS Class
GS_Class myGS;

void GS_Class::begin()
{
	//initialize serial streams	
	DEBUG_PORT.begin(DEBUG_PORT_BAUD);
	COMMAND_PORT.begin(COMMAND_PORT_BAUD);
	TELEM_PORT.begin(TELEM_PORT_BAUD);




	//wait for all serial ports to come online
	//while (!DEBUG_PORT);
	DEBUG_PORT.println(F("Initializing serial ports..."));
	while (!COMMAND_PORT)
	{
		DEBUG_PORT.println(F("Initializing command port..."));
		COMMAND_PORT.begin(COMMAND_PORT_BAUD);
		delay(500);
	}
	while (!TELEM_PORT)
	{
		DEBUG_PORT.println(F("Initializing telemetry port..."));
		TELEM_PORT.begin(TELEM_PORT_BAUD);
		delay(500);
	}
	DEBUG_PORT.println(F("\tAll ports sucessfully initialized"));




	//turn on PWR LED
	DEBUG_PORT.println(F("Turning on PWR LED..."));
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	DEBUG_PORT.println(F("\tPWR LED on"));




	//set analog to digital conversion resolution (in bits)
	DEBUG_PORT.println(F("Setting ADC resolution (default - 16 bits)..."));
	analogReadResolution(ANALOG_RESOLUTION);
	DEBUG_PORT.println(F("\tResolution set"));

	return;
}




int GS_Class::grabData_Radio()
{
	int result = myRadio.grabData_Radio();

	if (result == AIR_NEW_DATA)
	{
		//To Do
	}

	return result;
}




//send telemetry data to GS
void GS_Class::sendCommands()
{
	//update the radio's outgoing array with the propper information
	//To Do

	//send the telemetry data to GS
	myRadio.sendData();

	return;
}
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if TELEM_PORT_NUMBER == 0

void serialEvent()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 1

void serialEvent1()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 2

void serialEvent2()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 3

void serialEvent3()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)COMMAND_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 4

void serialEvent4()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 5

void serialEvent5()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#elif TELEM_PORT_NUMBER == 6

void serialEvent6()
{
	while (TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)TELEM_PORT.read();

		// add it to the inputString:
		if (!myRadio.stringComplete_Radio)
			myRadio.inputString_Radio += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == END_BYTE)
		{
			myRadio.stringComplete_Radio = true;
		}
	}

	return;
}

#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////