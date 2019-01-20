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

#include "AirComms.h"

#include "Shared_Tools.h"
#include "IFC_Serial.h"
#include "GS_Serial.h"




//create GPS class
airComms myRadio;




void airComms::begin(bool _is_IFC)
{
	//record if this is running on IFC or GS
	is_IFC = _is_IFC;

	//set the propper serial ports
	if (_is_IFC)
	{
		AIR_DEBUG_PORT = &IFC_DEBUG_PORT;
		AIR_COMMAND_PORT = &IFC_COMMAND_PORT;
		AIR_TELEM_PORT = &IFC_TELEM_PORT;
	}
	else
	{
		AIR_DEBUG_PORT = &GS_DEBUG_PORT;
		AIR_COMMAND_PORT = &GS_COMMAND_PORT;
		AIR_TELEM_PORT = &GS_TELEM_PORT;
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
			//reset the current value in the buffer
			outBuff[i + 2] = 0;

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
		if (is_IFC)
		{
			AIR_TELEM_PORT->write(packet[i]);
		}
		else
		{
			AIR_COMMAND_PORT->write(packet[i]);
		}
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

		//determine if the start byte is at the start of the packet
		if (buffIndex == 0)
		{
			//process the data if the packet is uncorrupted
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
	byte k = 0;

	//determine if a full packet was received
	if ((inputString_Radio.length() == BUFF_LEN) && (inputString_Radio[0] == START_BYTE) && (inputString_Radio[BUFF_LEN-1] == END_BYTE))
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