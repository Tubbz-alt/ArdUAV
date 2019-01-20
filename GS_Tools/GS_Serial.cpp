#include "GS_Serial.h"

#include "Shared_Tools.h"
#include "AirComms.h"




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if GS_TELEM_PORT_NUMBER == 0

void serialEvent()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 1

void serialEvent1()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 2

void serialEvent2()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 3

void serialEvent3()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_COMMAND_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 4

void serialEvent4()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 5

void serialEvent5()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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

#elif GS_TELEM_PORT_NUMBER == 6

void serialEvent6()
{
	while (GS_TELEM_PORT.available())
	{
		// get the new byte:
		char inChar = (char)GS_TELEM_PORT.read();

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