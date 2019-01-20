#include "neo6mGPS.h"

#include "IFC_Tools.h"
#include "IFC_Serial.h"
#include "Shared_Tools.h"




//create GPS class
neo6mGPS myGPS;




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin()
{
	// reserve 75 bytes for the inputString:
	inputString_GPS.reserve(75);

	//try to initialize the serial port if not already initialized - only try once
	if(!IFC_DEBUG_PORT)
		IFC_DEBUG_PORT.begin(DEBUG_PORT_BAUD);

	//setup GPS
	setupGPS();

	return;
}




//setup GPS and load non-default configuration settings
void neo6mGPS::setupGPS()
{
	IFC_DEBUG_PORT.println(F("\tStarting NEO-6M GPS auto-configuration..."));
	
	// Disable NMEA messages by sending appropriate packets.
	disableNmea();

	// Enable NMEA messages by sending appropriate packets.
	enableNmea();

	// Increase frequency to 100 ms.
	changeFrequency();

	// Switch the receiver serial to the wanted baudrate.
	changeBaudrate();

	IFC_DEBUG_PORT.println(F("\t\tFinished NEO-6M GPS auto-configuration!"));

	return;
}




//send a set of packets to the receiver to disable NMEA messages.
void neo6mGPS::disableNmea()
{
	IFC_DEBUG_PORT.println(F("\t\tTurning off all NMEA sentences..."));

	writeConfigPacket(turn_Off_GPGGA, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGLL, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGSA, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGLV, NMEA_LEN);
	writeConfigPacket(turn_Off_GPRMC, NMEA_LEN);
	writeConfigPacket(turn_Off_GPVTG, NMEA_LEN);

	return;
}




//send a set of packets to the receiver to enable NMEA messages.
void neo6mGPS::enableNmea()
{
	IFC_DEBUG_PORT.println(F("\t\tTurning on desired NMEA sentences..."));

	//comment or uncomment based on what sentences desired

	//writeConfigPacket(turn_On_GPGGA, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGLL, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGSA, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGLV, NMEA_LEN);
	writeConfigPacket(turn_On_GPRMC, NMEA_LEN);
	//writeConfigPacket(turn_On_GPVTG, NMEA_LEN);

	return;
}




//send a packet to the receiver to change baudrate to 115200.
void neo6mGPS::changeBaudrate()
{
	IFC_DEBUG_PORT.println(F("\t\tChanging baud rate..."));

	writeConfigPacket(changeBaud, BAUD_LEN);

	delay(100); // Little delay before flushing.
	IFC_GPS_PORT.flush();
	IFC_GPS_PORT.begin(115200); //reset serial port to new baud

	return;
}




//send a packet to the receiver to change frequency to 100 ms.
void neo6mGPS::changeFrequency()
{
	IFC_DEBUG_PORT.println(F("\t\tUpdating refresh frequency..."));

	writeConfigPacket(updateFreq, FREQ_LEN);

	return;
}




//update lat and lon in the GPS_data array
int neo6mGPS::grabData_GPS()
{
	int returnVal = GPS_NO_DATA;

	// print the string when a newline arrives:
	if (stringComplete_GPS)
	{
		buffIndex = inputString_GPS.indexOf("RMC");

		if (buffIndex == 3)
		{
			//optional debugging print
			//IFC_DEBUG_PORT.println(inputString_GPS);

			returnVal = extractData_GPS(buffIndex);
		}

		// clear the string:
		inputString_GPS = "";
		stringComplete_GPS = false;
	}

	//nothing found
	return returnVal;
}


//																			  00000000001111111111222222222233333333334444444444555555555566666666667
//																			  01234567890123456789012345678901234567890123456789012345678901234567890
//extract lat and lon data from the GPS stream buffer (Example NMEA Sentence: $GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57)
int neo6mGPS::extractData_GPS(byte startingIndex)
{
	if (inputString_GPS[startingIndex + 14] == 'A')
	{
		GPS_data[UTC_HOUR_POS] = ((inputString_GPS[startingIndex + 4] - '0') * 10) + (inputString_GPS[startingIndex + 5] - '0');

		GPS_data[UTC_MINUTE_POS] = ((inputString_GPS[startingIndex + 6] - '0') * 10) + (inputString_GPS[startingIndex + 7] - '0');

		GPS_data[UTC_SECOND_POS] = ((inputString_GPS[startingIndex + 8] - '0') * 10) + (inputString_GPS[startingIndex + 9] - '0') + ((inputString_GPS[startingIndex + 11] - '0') / 10) + ((inputString_GPS[startingIndex + 12] - '0') / 100);

		GPS_data[LAT_POS] = (((inputString_GPS[startingIndex + 16] - '0') * 10) + (inputString_GPS[startingIndex + 17] - '0'))																																														//degrees
						  + ((((inputString_GPS[startingIndex + 18] - '0') * 10) + (inputString_GPS[startingIndex + 19] - '0')) / 60.0)																																												//minutes
						  + ((((inputString_GPS[startingIndex + 20] - '0') * 10) + (inputString_GPS[startingIndex + 21] - '0') + ((inputString_GPS[startingIndex + 23] - '0') / 10.0) + ((inputString_GPS[startingIndex + 24] - '0') / 100.0) + ((inputString_GPS[startingIndex + 25] - '0') / 1000.0)) / 3600.0);	//seconds
	
		GPS_data[LON_POS] = (((inputString_GPS[startingIndex + 29] - '0') * 100) + ((inputString_GPS[startingIndex + 30] - '0') * 10) + (inputString_GPS[startingIndex + 31] - '0'))																																//degrees
						  + ((((inputString_GPS[startingIndex + 32] - '0') * 10) + (inputString_GPS[startingIndex + 33] - '0')) / 60.0)																																												//minutes
						  + ((((inputString_GPS[startingIndex + 35] - '0') * 10) + (inputString_GPS[startingIndex + 36] - '0') + ((inputString_GPS[startingIndex + 37] - '0') / 10.0) + ((inputString_GPS[startingIndex + 38] - '0') / 100.0) + ((inputString_GPS[startingIndex + 39] - '0') / 1000.0)) / 3600.0);	//seconds

		if (inputString_GPS[startingIndex + 27] == 'S')
		{
			GPS_data[LAT_POS] = -GPS_data[LAT_POS];
		}

		if (inputString_GPS[startingIndex + 41] == 'W')
		{
			GPS_data[LON_POS] = -GPS_data[LON_POS];
		}

		if (inputString_GPS[startingIndex + 43] != ',')
		{
			GPS_data[SPD_POS] = (inputString_GPS[startingIndex + 43] - '0') + ((inputString_GPS[startingIndex + 45] - '0') / 10) + ((inputString_GPS[startingIndex + 46] - '0') / 100) + ((inputString_GPS[startingIndex + 47] - '0') / 1000);
			
			if (inputString_GPS[startingIndex + 49] != ',')
			{
				//TO DO
				//GPS_data[COG_POS] = (inputString[startingIndex + 49] - '0')
			}
		}
		else if (inputString_GPS[startingIndex + 47] != ',')
		{
			//TO DO
		}
		else
		{
			//TO DO
		}

		return GPS_NEW_DATA;
	}
	else
	{
		return GPS_NO_DATA;
	}
}




//send the packet specified to the receiver.
void neo6mGPS::writeConfigPacket(byte packet[], byte len)
{
	for (byte i = 0; i < len; i++)
	{
		IFC_GPS_PORT.write(packet[i]);
	}

	return;
}



