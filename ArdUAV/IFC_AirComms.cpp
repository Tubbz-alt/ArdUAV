/////////////////////////////////////////////////////////////////////////////////////////
/*
   Command Array Anatomy:

   --------------------------------------------------------------------------------------
   Index Number - Command Type                    - Command Format
   --------------------------------------------------------------------------------------
		0       - pitch                           - unsigned 16-Bit Analog
		1       - roll                            - unsigned 16-Bit Analog
		2       - yaw                             - unsigned 16-Bit Analog
		3       - throttle                        - unsigned 16-Bit Analog
		4       - Autopilot Toggle                - unsigned 16-Bit  Boolean (0x0 or 0x1)
		5		- Pitch/Roll Limiter Toggle       - unsigned 16-Bit  Boolean (0x0 or 0x1)
		6       - Landing Gear Toggle             - unsigned 16-Bit  Boolean (0x0 or 0x1)
		7		- Flaps Toggle                    - unsigned 16-Bit  Boolean (0x0 or 0x1)
		8       - Unused                          - Unused
		9       - Unused                          - Unused
		10      - Unused                          - Unused
		11      - Unused                          - Unused
		12      - Unused                          - Unused
		13      - Unused                          - Unused
		14      - Unused                          - Unused
		15      - Unused                          - Unused
		16      - Unused                          - Unused
		17      - Unused                          - Unused
		18      - Unused                          - Unused
		19      - Unused                          - Unused
*/
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/*
   Telemetry Array Anatomy:

   --------------------------------------------------------------------------------------
   Index Number - Telemetry Type                  - Telemetry Format
   --------------------------------------------------------------------------------------
		0       - Pitot Velocity (m/s) *100       - unsigned 16-bit  Analog
		1       - Converted Altitude (m) * 100    - unsigned 16-bit  Analog
		2       - Pitch Angle (degrees) * 100     - signed 16-bit  Analog
		3       - Roll Angle (degrees) * 100      - signed 16-bit  Analog
		4       - Latitude (dd) * 100             - signed 16-bit  Analog
		5       - Longitude (dd) * 100            - signed 16-bit  Analog
		6       - Unused                          - Unused
		7       - Unused                          - Unused
		8       - Unused                          - Unused
		9       - Unused                          - Unused
		10      - Unused                          - Unused
		11      - Unused                          - Unused
		12      - Unused                          - Unused
		13      - Unused                          - Unused
		14      - Unused                          - Unused
		15      - Unused                          - Unused
		16      - Unused                          - Unused
		17      - Unused                          - Unused
		18      - Unused                          - Unused
		19      - Unused                          - Unused
*/
/////////////////////////////////////////////////////////////////////////////////////////

#include "IFC_AirComms.h"

#include "IFC_Tools.h"
#include "Shared_Tools.h"




//create GPS class
airComms myRadio;




void airComms::begin()
{
	//try to initialize the serial port if not already initialized - only try once
	if (!DEBUG_PORT)
		DEBUG_PORT.begin(DEBUG_PORT_BAUD);

	//initialize the rest of the ports
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

	return;
}




//send data from outgoingArray
void airComms::sendData()
{
	byte k = 0;

	//"build" the packet to transmit
	outBuff[0] = START_BYTE;
	outBuff[1] = 0;

	//break all integers from outgoingArray apart into bytes and stuff them all into the packet
	for (byte i = 0; i < (AIR_DATA_LEN * 2); i++)
	{
		if (i % 2)
		{
			//stuff LSB into the array
			outBuff[i + 2] = outgoingArray[k] & 0xFF;

			k++;
		}
		else
		{
			//stuff MSB into the array
			outBuff[i + 2] = (outgoingArray[k] >> 8) & 0xFF;
		}
	}

	outBuff[BUFF_LEN - 1] = END_BYTE;
	outBuff[1] = findChecksum(outBuff);

	//send the packet one byte at a time
	writePacket(outBuff);

	return;
}




//send the packet one byte at a time
void airComms::writePacket(byte packet[])
{
	for (byte i = 0; i < BUFF_LEN; i++)
	{
#ifdef IFC
		TELEM_PORT.write(packet[i]);
#else
		COMMAND_PORT.write(packet[i]);
#endif
	}

	return;
}




//update incomingArray with new data if available
byte airComms::grabData_Radio()
{
	byte buffIndex;
	int returnVal = AIR_NO_DATA;

	// print the string when a newline arrives:
	if (stringComplete_Radio)
	{
		buffIndex = inputString_Radio.indexOf(START_BYTE);

		if (buffIndex == 0)
		{
			//optional debugging print
			//DEBUG_PORT.println(inputString_Radio);

			returnVal = extractData_Radio(buffIndex);
		}

		// clear the string:
		inputString_Radio = "";
		stringComplete_Radio = false;
	}

	//nothing found
	return returnVal;
}




//unpack and save received data packet
int airComms::extractData_Radio(byte startingIndex)
{
	byte receivedChecksum;
	byte calculatedChecksum;
	byte k = 0;

	//determine if a full packet was received
	if (inputString_Radio.length() == BUFF_LEN)
	{
		//calculate and compare checksums (received vs calculated)
		receivedChecksum = inputString_Radio[1];
		inputString_Radio[1] = 0;
		calculatedChecksum = findChecksum(inputString_Radio);

		if (receivedChecksum == calculatedChecksum)
		{
			//shuffle all bytes back into ints and save in incomingArray
			for (byte i = 2; i < (((AIR_DATA_LEN * 2) - 1) + 2); i += 2)
			{
				incomingArray[k] = (inputString_Radio[i] << 8) | inputString_Radio[i + 1];

				k++;
			}

			return AIR_NEW_DATA;
		}
		else
		{
			return AIR_CHECKSUM_ERROR;
		}
	}
	else
	{
		return AIR_PAYLOAD_ERROR;
	}
}




//find 8-bit checksum of message
byte airComms::findChecksum(byte buff[])
{
	//initializw checksum
	byte checksum = 0;

	//compute checksum
	for (byte i = 0; i < BUFF_LEN; i++)
	{
		checksum = checksum + buff[i];
	}

	checksum = (~checksum) + 1;

	//checksum updated
	return checksum;
}




//find 8-bit checksum of message
byte airComms::findChecksum(String buff)
{
	//initializw checksum
	byte checksum = 0;

	//compute checksum
	for (byte i = 0; i < BUFF_LEN; i++)
	{
		checksum = checksum + buff[i];
	}

	checksum = (~checksum) + 1;

	//checksum updated
	return checksum;
}