#include "IFC_Tools.h"
#include "IFC_Serial.h"

#include "Shared_Tools.h"




/////////////////////////////////////////////////////////////////////////////////////////
//sensor/actuator classes
Adafruit_BNO055 bno = Adafruit_BNO055(&Wire, 55, BNO055_ADDRESS_A);
LIDARLite myLidarLite(&Wire);
Servo rudder;
Servo elevator;
Servo aileron_L;
Servo aileron_R;
Servo throttle;
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//IFC Class
IFC_Class myIFC;

void IFC_Class::begin()
{
	//initialize variables
	dataTimestamp_IMU = 0;
	controlInputs.limiter_enable = false;




	//initialize serial streams
	IFC_DEBUG_PORT.begin(DEBUG_PORT_BAUD);
	IFC_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
	IFC_GPS_PORT.begin(GPS_PORT_BAUD);
	IFC_TELEM_PORT.begin(TELEM_PORT_BAUD);




	//wait for all serial ports to come online
	IFC_DEBUG_PORT.println(F("Initializing serial ports..."));
	IFC_DEBUG_PORT.print(F("Initializing command port at Serial")); IFC_DEBUG_PORT.print(IFC_COMMAND_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
	IFC_DEBUG_PORT.print(F("Initializing GPS port at Serial")); IFC_DEBUG_PORT.print(IFC_GPS_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
	IFC_DEBUG_PORT.print(F("Initializing telemetry at Serial")); IFC_DEBUG_PORT.print(IFC_TELEM_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
	//while (!DEBUG_PORT);
	while (!IFC_COMMAND_PORT)
	{
		IFC_DEBUG_PORT.print(F("Initializing command port at Serial")); IFC_DEBUG_PORT.print(IFC_COMMAND_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
		IFC_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
		delay(500);
	}
	while (!IFC_GPS_PORT)
	{
		IFC_DEBUG_PORT.print(F("Initializing GPS port at Serial")); IFC_DEBUG_PORT.print(IFC_GPS_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
		IFC_GPS_PORT.begin(GPS_PORT_BAUD);
		delay(500);
	}
	while (!IFC_TELEM_PORT)
	{
		IFC_DEBUG_PORT.print(F("Initializing telemetry at Serial")); IFC_DEBUG_PORT.print(IFC_TELEM_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
		IFC_TELEM_PORT.begin(TELEM_PORT_BAUD);
		delay(500);
	}
	IFC_DEBUG_PORT.println(F("\tAll ports sucessfully initialized"));




	//turn on PWR LED
	IFC_DEBUG_PORT.println(F("Turning on PWR LED..."));
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	IFC_DEBUG_PORT.println(F("\tPWR LED on"));




	//set analog to digital conversion resolution (in bits)
	IFC_DEBUG_PORT.println(F("Setting ADC resolution (default - 16 bits)..."));
	analogReadResolution(ANALOG_RESOLUTION);
	IFC_DEBUG_PORT.println(F("\tResolution set"));




	//initialize AirComs
	IFC_DEBUG_PORT.println(F("Initializing AirComms..."));
	myRadio.begin(true);
	IFC_DEBUG_PORT.println(F("\tAirComms initialized"));




	//initialize GPS
	IFC_DEBUG_PORT.println(F("Initializing GPS..."));
	myGPS.begin();
	IFC_DEBUG_PORT.println(F("\tGPS initialized"));




	//initialize the IMU and wait for it to "boot up"
	IFC_DEBUG_PORT.println(F("Initializing IMU..."));
	if (!bno.begin())
	{
		//there was a problem detecting the BNO055 ... check your connections
		IFC_DEBUG_PORT.println(F("\tOoops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
		
		//fatal error - halt code execution
		while(1);
	}
	else
	{
		delay(1000);
		bno.setExtCrystalUse(true);
		IFC_DEBUG_PORT.println(F("\tBNO055 successfully initialized"));
	}




	//initialize the LiDAR for long distance readings
	IFC_DEBUG_PORT.println(F("Initializing LiDAR altimeter..."));
	myLidarLite.begin(0, true);
	myLidarLite.configure(0);
	LiDAR_Counter = 0;
	IFC_DEBUG_PORT.println(F("\tLiDAR altimeter initialized..."));




	//initialize each individual servo to their respective center position
	IFC_DEBUG_PORT.println(F("Initializing Servos..."));
	rudder.attach(RUDDER_PIN);
	elevator.attach(ELEVATOR_PIN);
	aileron_L.attach(L_AILERON_PIN);
	aileron_R.attach(R_AILERON_PIN);

	rudder.writeMicroseconds(RUDDER_MID);
	elevator.writeMicroseconds(ELEVATOR_MID);
	aileron_L.writeMicroseconds(AILERON_MID);
	aileron_R.writeMicroseconds(AILERON_MID);
	IFC_DEBUG_PORT.println(F("\tServos initialized..."));




	//initialize the ESC
	IFC_DEBUG_PORT.println(F("Initializing ESC..."));
	throttle.attach(THROTTLE_PIN);
	throttle.write(THROTTLE_MIN);
	IFC_DEBUG_PORT.println(F("\tESC initialized..."));

	return;
}




int IFC_Class::grabData_GPS()
{
	//get data from the GPS
	int result = myGPS.grabData_GPS();

	//if a new packet was processed, update the telemetry struct
	if (result == GPS_NEW_DATA)
	{
		telemetry.latitude = myGPS.GPS_data[LAT_POS];
		telemetry.longitude = myGPS.GPS_data[LON_POS];
		telemetry.UTC_year = (uint16_t)myGPS.GPS_data[UTC_YEAR_POS];
		telemetry.UTC_month = (uint16_t)myGPS.GPS_data[UTC_MONTH_POS];
		telemetry.UTC_day = (uint16_t)myGPS.GPS_data[UTC_DAY_POS];
		telemetry.UTC_hour = (uint16_t)myGPS.GPS_data[UTC_HOUR_POS];
		telemetry.UTC_minute = (uint16_t)myGPS.GPS_data[UTC_MINUTE_POS];
		telemetry.UTC_second = (uint16_t)myGPS.GPS_data[UTC_SECOND_POS];
		telemetry.speedOverGround = myGPS.GPS_data[SPD_POS];
		telemetry.courseOverGround = myGPS.GPS_data[COG_POS];
	}

	return result;
}




int IFC_Class::grabData_IMU()
{
	//get a new sensor event
	sensors_event_t event;
	bno.getEvent(&event);

	//get course, pitch, roll angles in degrees
	telemetry.courseAngle = event.orientation.x;
	telemetry.rollAngle = event.orientation.z;
	telemetry.pitchAngle = event.orientation.y;

	//convert Euler angles from degrees to radians - ONLY USED FOR LiDAR CORRECTION
	telemetry.convertedRoll = (telemetry.rollAngle) * (M_PI / 180);
	telemetry.convertedPitch = (telemetry.pitchAngle) * (M_PI / 180);

	//timestamp the new data
	dataTimestamp_IMU = millis();

	return 1;
}




int IFC_Class::grabData_LiDAR()
{
	//get altitude readings - including periodic bias correction
	if (LiDAR_Counter >= 100)
	{
		telemetry.altitude = myLidarLite.distance();
		LiDAR_Counter = 0;
	}
	else
	{
		telemetry.altitude = myLidarLite.distance(false);
		LiDAR_Counter = LiDAR_Counter + 1;
	}

	telemetry.convertedAltitude = telemetry.altitude * cos(telemetry.convertedRoll) * cos(telemetry.convertedPitch);

	return 1;
}




int IFC_Class::grabData_Pitot()
{
	uint16_t rawValue = analogRead(PITOT_PIN);

	telemetry.velocity = rawValue; //(2.0 / 65251.0) * (-24111.0 + sqrt(-5560435134679.0 + 163127500.0 * rawValue));

	return 1;
}




int IFC_Class::grabData_Radio()
{
	int result;

	//check to see if there is a loss of radio link between GS and IFC
	checkRadioLink();

	//see if new data is available
	result = myRadio.grabData_Radio();

	//if a new packet was processed, update the controlInputs struct
	if (result == AIR_NEW_DATA)
	{
		//update controlInputs struct so that the next time the servos can be updated with the latest positions
		controlInputs.pitch_command = myRadio.incomingArray[AIR_PITCH_INDEX];
		controlInputs.roll_command = myRadio.incomingArray[AIR_ROLL_INDEX];
		controlInputs.yaw_command = myRadio.incomingArray[AIR_YAW_INDEX];
		controlInputs.throttle_command = myRadio.incomingArray[AIR_THROTTLE_INDEX];
		controlInputs.autopilot_command = myRadio.incomingArray[AIR_AUTOPILOT_INDEX];
		controlInputs.limiter_command = myRadio.incomingArray[AIR_LIMITER_INDEX];
		controlInputs.landingGear_command = myRadio.incomingArray[AIR_LANDING_GEAR_INDEX];
		controlInputs.flaps_command = myRadio.incomingArray[AIR_FLAPS_INDEX];

		//tweak the contents of controlInputs to keep the plane from unsafe maneuvers
		bankPitchLimiter(controlInputs.limiter_enable, true);
	}
	else if (!linkConnected)
	{
		//tweak the contents of controlInputs to keep the plane from unsafe maneuvers
		//update servos with new values automatically if there is a loss of link
		bankPitchLimiter(controlInputs.limiter_enable, false);
	}

	return result;
}




//send telemetry data to GS
void IFC_Class::sendTelem()
{
	//use timer to send commands to the plane at a fixed rate
	currentTime_Telem = millis();
	if ((currentTime_Telem - timeBench_Telem) >= REPORT_TELEM_PERIOD)
	{
		//reset timer
		timeBench_Telem = currentTime_Telem;

		//update the radio's outgoing array with the propper information
		myRadio.outgoingArray[AIR_PITOT_INDEX] = (int16_t)(telemetry.velocity * 100);
		myRadio.outgoingArray[AIR_ALTITUDE_INDEX] = (int16_t)(telemetry.altitude * 100);
		myRadio.outgoingArray[AIR_PITCH_ANGLE_INDEX] = (int16_t)(telemetry.pitchAngle * 100);
		myRadio.outgoingArray[AIR_ROLL_ANGLE_INDEX] = (int16_t)(telemetry.rollAngle * 100);
		myRadio.outgoingArray[AIR_LATITUDE_INDEX] = (int16_t)(telemetry.latitude * 100);
		myRadio.outgoingArray[AIR_LONGITUDE_INDEX] = (int16_t)(telemetry.longitude * 100);
		myRadio.outgoingArray[AIR_UTC_YEAR_INDEX] = telemetry.UTC_year;
		myRadio.outgoingArray[AIR_UTC_MONTH_INDEX] = telemetry.UTC_month;
		myRadio.outgoingArray[AIR_UTC_DAY_INDEX] = telemetry.UTC_day;
		myRadio.outgoingArray[AIR_UTC_HOUR_INDEX] = telemetry.UTC_hour;
		myRadio.outgoingArray[AIR_UTC_MINUTE_INDEX] = telemetry.UTC_minute;
		myRadio.outgoingArray[AIR_UTC_SECOND_INDEX] = telemetry.UTC_second;
		myRadio.outgoingArray[AIR_SOG_INDEX] = (int16_t)(telemetry.speedOverGround * 100);
		myRadio.outgoingArray[AIR_COG_INDEX] = (int16_t)(telemetry.courseOverGround * 100);

		//send the telemetry data to GS
		myRadio.sendData();
	}

	return;
}




//update servo positions (use controlInputs commands)
void IFC_Class::updateServos()
{
	//update ESC speed
	throttle.write(constrain(controlInputs.throttle_command, THROTTLE_MIN, THROTTLE_MAX));

	//update servo positions
	elevator.writeMicroseconds(constrain(controlInputs.pitch_command, ELEVATOR_MIN, ELEVATOR_MAX));
	rudder.writeMicroseconds(constrain(controlInputs.yaw_command, RUDDER_MIN, RUDDER_MAX));
	aileron_L.writeMicroseconds(constrain(controlInputs.roll_command, AILERON_MIN, AILERON_MAX));
	aileron_R.writeMicroseconds(constrain(controlInputs.roll_command, AILERON_MIN, AILERON_MAX));

	return;
}




//update a single servo's position (use controlInputs commands)
void IFC_Class::updateSingleServo(byte INDEX, uint16_t value)
{
	if (INDEX == THROTTLE_INDEX)
	{
		throttle.write(constrain(value, THROTTLE_MIN, THROTTLE_MAX));
	}
	else if (INDEX == ELEVATOR_INDEX)
	{
		elevator.writeMicroseconds(constrain(value, ELEVATOR_MIN, ELEVATOR_MAX));
	}
	else if (INDEX == RUDDER_INDEX)
	{
		rudder.writeMicroseconds(constrain(value, RUDDER_MIN, RUDDER_MAX));
	}
	else if (INDEX == AILERON_INDEX)
	{
		aileron_L.writeMicroseconds(constrain(value, AILERON_MIN, AILERON_MAX));
		aileron_R.writeMicroseconds(constrain(value, AILERON_MIN, AILERON_MAX));
	}
	else
	{
		IFC_DEBUG_PORT.println(F("Servo Not recognized"));
	}

	return;
}




//check to see if there is a loss of radio link between GS and IFC
bool IFC_Class::checkRadioLink()
{
	//see if new data has arrived - if not, test if the timeout condition is met
	if (!myRadio.arrayComplete_Radio)
	{
		//timer to see if there is a loss of link between GS and plane
		currentTime_Commands = millis();

		if (!noPacketFlag)
		{
			timeBench_Commands = currentTime_Commands;
			noPacketFlag = true;
		}

		if ((currentTime_Commands - timeBench_Commands) >= LOSS_LINK_TIMEOUT)
		{
			//link is severed - unset flag if not alread unset
			linkConnected = false;
			IFC_DEBUG_PORT.println("loss link");
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




//keep the plane from pitching or rolling too much in any direction
void IFC_Class::bankPitchLimiter(bool enable, bool _linkConnected)
{
	//determine if user wants to engage the bank and pitch limiter
	if (enable)
	{
		//determine if the radio link is healthy and connected
		if (_linkConnected)
		{
			//update struct based on euler angles
			updateControlsLimiter(PITCH_AXIS);	//pitch
			updateControlsLimiter(ROLL_AXIS);	//roll
		}
		else
		{
			//use timer to send commands to the servos at a fixed rate
			currentTime_Limiter = millis();
			if ((currentTime_Limiter - timeBench_Limiter) >= REPORT_COMMANDS_PERIOD)
			{
				//reset timer
				timeBench_Limiter = currentTime_Limiter;

				//update struct based on euler angles
				updateControlsLimiter(PITCH_AXIS);	//pitch
				updateControlsLimiter(ROLL_AXIS);	//roll

				//update servo positions (use controlInputs commands)
				updateServos();
			}
		}
	}

	return;
}




//update struct based on euler angles
void IFC_Class::updateControlsLimiter(bool axis)
{
	//minimum servo command to get back to safe flight
	uint16_t minServoCommand;

	//determine if the current IMU data is old - if so, get new IMU data
	if ((millis() - dataTimestamp_IMU) >= LIMITER_PERIOD)
	{
		//grab IMU data
		grabData_IMU();
	}

	//determine if pitch or roll should be tweaked (axis==true --> pitch, axis==false --> roll)
	if (axis == PITCH_AXIS)
	{
		//determine if the current pitch angle is too low
		if (telemetry.pitchAngle <= UNSAFE_PITCH_UP)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.pitchAngle), abs(UNSAFE_PITCH_UP), abs(MAX_PITCH_UP), ELEVATOR_MID, ELEVATOR_MIN), ELEVATOR_MIN, ELEVATOR_MID);
			
			//determine if minimum servo command is less than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand < controlInputs.pitch_command)
			{
				//update controlInputs struct
				controlInputs.pitch_command = minServoCommand;
			}
		}
		//determine if the current pitch angle is too high
		else if (telemetry.pitchAngle >= UNSAFE_PITCH_DOWN)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.pitchAngle), abs(UNSAFE_PITCH_DOWN), abs(MAX_PITCH_DOWN), ELEVATOR_MID, ELEVATOR_MAX), ELEVATOR_MID, ELEVATOR_MAX);
		
			//determine if minimum servo command is more than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand > controlInputs.pitch_command)
			{
				//update controlInputs struct
				controlInputs.pitch_command = minServoCommand;
			}
		}
	}
	else if (axis == ROLL_AXIS)
	{
		//determine if the current roll angle is too low
		if (telemetry.rollAngle <= UNSAFE_ROLL_R)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.rollAngle), abs(UNSAFE_ROLL_R), abs(MAX_ROLL_R), AILERON_MID, AILERON_MAX), AILERON_MID, AILERON_MAX);

			//determine if minimum servo command is more than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand > controlInputs.roll_command)
			{
				//update controlInputs struct
				controlInputs.roll_command = minServoCommand;
			}
		}
		//determine if the current roll angle is too high
		else if (telemetry.rollAngle >= UNSAFE_ROLL_L)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.rollAngle), abs(UNSAFE_ROLL_L), abs(MAX_ROLL_L), AILERON_MID, AILERON_MIN), AILERON_MIN, AILERON_MID);

			//determine if minimum servo command is less than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand < controlInputs.roll_command)
			{
				//update controlInputs struct
				controlInputs.roll_command = minServoCommand;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
