#include "IFC_Serial.h"
#include "IFC_Tools.h"

#include "Shared_Tools.h"
#include "AirComms.h"
#include "neo6mGPS.h"




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if IFC_GPS_PORT_NUMBER == 0

void serialEvent()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 1

void serialEvent1()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 2

void serialEvent2()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete)
			myGPS.inputString += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 3

void serialEvent3()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 4

void serialEvent4()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 5

void serialEvent5()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#elif IFC_GPS_PORT_NUMBER == 6

void serialEvent6()
{
	while (IFC_GPS_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_GPS_PORT.read();

		// add it to the inputString:
		if (!myGPS.stringComplete_GPS)
			myGPS.inputString_GPS += inChar;

		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '\n')
		{
			myGPS.stringComplete_GPS = true;
		}
	}

	return;
}

#endif




#if IFC_COMMAND_PORT_NUMBER == 0

void serialEvent()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 1

void serialEvent1()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 2

void serialEvent2()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 3

void serialEvent3()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 4

void serialEvent4()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 5

void serialEvent5()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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

#elif IFC_COMMAND_PORT_NUMBER == 6

void serialEvent6()
{
	while (IFC_COMMAND_PORT.available())
	{
		// get the new byte:
		char inChar = (char)IFC_COMMAND_PORT.read();

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