#include "GS_Serial.h"

#include "Shared_Tools.h"
#include "GS_Tools.h"
#include "AirComms.h"




void commEvent()
{
	while (GS_TELEM_PORT.available())
	{
		//add it to the inputString if the packet isn't complete yet:
		if (!myRadio.arrayComplete_Radio)
		{
			//get the new byte:
			byte inByte = GS_TELEM_PORT.read();

			//test if the index counter is still in the array bound (don't want to access things outside of our array)
			if (myRadio.inputArray_CurrentIndex < BUFF_LEN)
			{
				//add char to input string buffer
				myRadio.inputArray_Radio[myRadio.inputArray_CurrentIndex] = inByte;

				//increment the array counter
				myRadio.inputArray_CurrentIndex++;

				//test if the length of the string is at least one
				if (myRadio.inputArray_CurrentIndex >= 1)
				{
					//if the first byte isn't correct, reset the index counter and start over
					if (myRadio.inputArray_Radio[0] != START_BYTE)
					{
						//reset the index counter
						myRadio.inputArray_CurrentIndex = 0;

						//reset the whole array
						for (byte i = 0; i < BUFF_LEN; i++)
							myRadio.inputArray_Radio[i] = 0;
					}
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
		if (myRadio.inputArray_CurrentIndex == BUFF_LEN)
		{
			//check to see if the char in the (BUFF_LEN - 1) position is correctly the value of END_BYTE
			if (myRadio.inputArray_Radio[BUFF_LEN - 1] == END_BYTE)
			{
				//optional debugging prints
				////////////////////////////////////////////////////////////////////////////////////////////////////////
				/*for (byte i = 0; i < BUFF_LEN; i++)
				{
					Serial.print(myRadio.inputArray_Radio[i]); Serial.print(" ");
				}
				Serial.println();*/
				////////////////////////////////////////////////////////////////////////////////////////////////////////

				//set this flag to true so the data can be processed
				myRadio.arrayComplete_Radio = true;
			}
			else
			{
				//reset the index counter
				myRadio.inputArray_CurrentIndex = 0;

				//reset the whole array
				for (byte i = 0; i < BUFF_LEN; i++)
					myRadio.inputArray_Radio[i] = 0;
			}
		}
	}
}




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if GS_TELEM_PORT_NUMBER == 0
void serialEvent()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 1
void serialEvent1()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 2
void serialEvent2()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 3
void serialEvent3()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 4
void serialEvent4()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 5
void serialEvent5()
{
	commEvent();
}

#elif GS_TELEM_PORT_NUMBER == 6
void serialEvent6()
{
	commEvent();
}

#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
