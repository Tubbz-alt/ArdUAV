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
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 1

void serialEvent1()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 2

void serialEvent2()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 3

void serialEvent3()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 4

void serialEvent4()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 5

void serialEvent5()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#elif GS_TELEM_PORT_NUMBER == 6

void serialEvent6()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.stringComplete_Radio)
		{
			//get the new byte:
			char inChar = (char)GS_TELEM_PORT.read();

			//add char to input string buffer
			myRadio.inputString_Radio += inChar;

			//test if the length of the string is at least one
			if (myRadio.inputString_Radio.length() >= 1)
			{
				//if the first byte isn't correct, clear the string buffer and start over
				if (myRadio.inputString_Radio[0] != START_BYTE)
				{
					//clear the string buffer
					myRadio.inputString_Radio = "";
				}
			}
		}
		else
		{
			//not time to read from buffer - need to finish processing the completed packet contained in inputString_Radio first
			//break the loop - otherwise "while (IFC_COMMAND_PORT.available())" could be an infinite blocking loop if bytes aren't being read from the port
			break;
		}

		//check size of input buffer to see if it is big enough to fit a full packet
		if (myRadio.inputString_Radio.length() >= BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputString_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				/*for (byte i = 0; i < myRadio.inputString_Radio.length(); i++)
				{
					Serial.print((int)myRadio.inputString_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/

				//set this flag to true so the data can be processed
				myRadio.stringComplete_Radio = true;
			}
			else
			{
				//clear the string buffer and start over
				myRadio.inputString_Radio = "";
			}
		}
	}

	return;
}

#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
