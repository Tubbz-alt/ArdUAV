#include "IFC_Tools.h"
#include "IFC_Serial.h"
#include "Shared_Tools.h"
#include "SerialTransfer.h"
#include "neo6mGPS.h"




#if USE_IMU
void IFC_Class::pollIMU()
{
	//get IMU data and convert to degrees
	auto vect = bno.getQuat().toEuler();
	telemetry.courseAngleIMU = vect.x() * (180 / M_PI);
	telemetry.pitchAngle     = vect.z() * (180 / M_PI);
	telemetry.rollAngle      = vect.y() * (180 / M_PI);

	//convert Euler angles from degrees to radians - ONLY USED FOR LiDAR CORRECTION
	convertedPitch = vect.z();
	convertedRoll  = vect.y();

	//timestamp the new data - regardless of where this function was called
	imuTimer.start();
}




//keep the plane from pitching or rolling too much in any direction
void IFC_Class::bankPitchLimiter()
{
	if (linkConnected)
	{
		updateControlsLimiter(PITCH_AXIS);
		updateControlsLimiter(ROLL_AXIS);
	}
	else
	{
		if (limiterTimer.fire())
		{
			updateControlsLimiter(PITCH_AXIS);
			updateControlsLimiter(ROLL_AXIS);

			updateServos();
		}
	}
}




//update struct based on euler angles
void IFC_Class::updateControlsLimiter(const bool& axis)
{
	//minimum servo command to get back to safe flight
	uint16_t minServoCommand;

	//determine if the current IMU data is old - if so, get new IMU data
	if (imuTimer.fire())
		pollIMU();

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
				controlInputs.pitch_command = minServoCommand;
		}
		//determine if the current pitch angle is too high
		else if (telemetry.pitchAngle >= UNSAFE_PITCH_DOWN)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.pitchAngle), abs(UNSAFE_PITCH_DOWN), abs(MAX_PITCH_DOWN), ELEVATOR_MID, ELEVATOR_MAX), ELEVATOR_MID, ELEVATOR_MAX);

			//determine if minimum servo command is more than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand > controlInputs.pitch_command)
				controlInputs.pitch_command = minServoCommand;
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
				controlInputs.roll_command = minServoCommand;
		}
		//determine if the current roll angle is too high
		else if (telemetry.rollAngle >= UNSAFE_ROLL_L)
		{
			//determine minimum servo command to get back to safe flight
			minServoCommand = constrain(map(abs(telemetry.rollAngle), abs(UNSAFE_ROLL_L), abs(MAX_ROLL_L), AILERON_MID, AILERON_MIN), AILERON_MIN, AILERON_MID);

			//determine if minimum servo command is less than current servo command - if so, replace current command with minimum servo command
			if (minServoCommand < controlInputs.roll_command)
				controlInputs.roll_command = minServoCommand;
		}
	}
}
#endif




#if USE_GPS
bool IFC_Class::pollGPS()
{
	//if a new packet was processed, update the telemetry struct
	if (gps.available())
	{
		if (gps.navStatus == 'A')
		{
			telemetry.latitude         = gps.lat_dd;
			telemetry.longitude        = gps.lon_dd;
			telemetry.UTC_year         = gps.utc_year;
			telemetry.UTC_month        = gps.utc_month;
			telemetry.UTC_day          = gps.utc_day;
			telemetry.UTC_hour         = gps.utc_hour;
			telemetry.UTC_minute       = gps.utc_min;
			telemetry.UTC_second       = gps.utc_sec;
			telemetry.speedOverGround  = gps.sog_knots;
			telemetry.courseOverGround = gps.cog_true;

			telemetry.validFlags = telemetry.validFlags | 0x6;
		}
		else
			telemetry.validFlags = telemetry.validFlags & !0x6;

		lossGPSTimer.start();
		gpsConnected = true;
		telemetry.validFlags = telemetry.validFlags | 0x8;
	}

	return gpsFailover();
}




bool IFC_Class::gpsFailover()
{
	if (lossGPSTimer.fire(false))
	{
		gpsConnected = false;
		telemetry.validFlags = telemetry.validFlags & !0x8;
	}

	return gpsConnected;
}
#endif




#if USE_PITOT
void IFC_Class::pollPitot()
{
	uint16_t rawValue = analogRead(PITOT_PIN);

	telemetry.velocity = rawValue; //(2.0 / 65251.0) * (-24111.0 + sqrt(-5560435134679.0 + 163127500.0 * rawValue));
}
#endif




#if USE_LIDAR
void IFC_Class::lidarEvent()
{
	if (lidarTransfer.available())
	{
		//update controlInputs struct so that the next time the servos can be updated with the latest positions
		lidarTransfer.rxObj(telemetry.altitude);

		//use trig to find the triangulated elevation if the LiDAR sensor is not stabilized with a gimbal
		if (LIDAR_FIXED_MOUNT)
			telemetry.altitude = telemetry.altitude * cos(convertedRoll) * cos(convertedPitch);
		else
			telemetry.altitude = telemetry.altitude;
	}
}
#endif




void IFC_Class::begin()
{
	//initialize variables
	controlInputs.limiter_enable = false;
	controlInputs.manual_control_enable = true;




	//initialize serial streams
	IFC_COMMAND_PORT.begin(COMMAND_PORT_BAUD);

#if USE_DEBUG
	IFC_DEBUG_PORT.begin(DEBUG_PORT_BAUD);
#endif

#if USE_TELEM
	IFC_TELEM_PORT.begin(TELEM_PORT_BAUD);
#endif

#if USE_GPS
	IFC_GPS_PORT.begin(GPS_PORT_BAUD);
#endif

#if USE_LIDAR
	IFC_LIDAR_PORT.begin(LIDAR_PORT_BAUD);
#endif




	//wait for all serial ports to come online
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing serial ports..."));
	IFC_DEBUG_PORT.print(F("Initializing command port at Serial")); IFC_DEBUG_PORT.print(IFC_COMMAND_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
	IFC_DEBUG_PORT.print(F("Initializing GPS port at Serial"));     IFC_DEBUG_PORT.print(IFC_GPS_PORT_NUMBER);      IFC_DEBUG_PORT.println(F("..."));
	IFC_DEBUG_PORT.print(F("Initializing LiDAR port at Serial"));   IFC_DEBUG_PORT.print(IFC_LIDAR_PORT_NUMBER);    IFC_DEBUG_PORT.println(F("..."));
	IFC_DEBUG_PORT.print(F("Initializing telemetry at Serial"));    IFC_DEBUG_PORT.print(IFC_TELEM_PORT_NUMBER);    IFC_DEBUG_PORT.println(F("..."));
#endif

	while (!IFC_COMMAND_PORT)
	{
#if USE_DEBUG
		IFC_DEBUG_PORT.print(F("Initializing command port at Serial")); IFC_DEBUG_PORT.print(IFC_COMMAND_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
#endif
		IFC_COMMAND_PORT.begin(COMMAND_PORT_BAUD);
		delay(500);
	}

#if USE_GPS
	while (!IFC_GPS_PORT)
	{
#if USE_DEBUG
		IFC_DEBUG_PORT.print(F("Initializing GPS port at Serial")); IFC_DEBUG_PORT.print(IFC_GPS_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
#endif
		IFC_GPS_PORT.begin(GPS_PORT_BAUD);
		delay(500);
	}
#endif

#if USE_LIDAR
	while (!IFC_LIDAR_PORT)
	{
#if USE_DEBUG
		IFC_DEBUG_PORT.print(F("Initializing LiDAR port at Serial")); IFC_DEBUG_PORT.print(IFC_LIDAR_PORT);  IFC_DEBUG_PORT.println(F("..."));
#endif
		IFC_LIDAR_PORT.begin(LIDAR_PORT_BAUD);
		delay(500);
	}
#endif

#if USE_TELEM
	while (!IFC_TELEM_PORT)
	{
#if USE_DEBUG
		IFC_DEBUG_PORT.print(F("Initializing telemetry at Serial")); IFC_DEBUG_PORT.print(IFC_TELEM_PORT_NUMBER);  IFC_DEBUG_PORT.println(F("..."));
#endif
		IFC_TELEM_PORT.begin(TELEM_PORT_BAUD);
		delay(500);
	}
#endif

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tAll ports sucessfully initialized"));
#endif




	//turn on PWR LED
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Turning on PWR LED..."));
#endif

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tPWR LED on"));
#endif




	//set analog to digital conversion resolution (in bits)
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Setting ADC resolution (default - 16 bits)..."));
#endif

	analogReadResolution(ANALOG_RESOLUTION);

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tResolution set"));
#endif




	//initialize transfer classes
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing transfer classes..."));
#endif

	commandTransfer.begin(IFC_COMMAND_PORT);

#if USE_TELEM
	telemetryTransfer.begin(IFC_TELEM_PORT);
#endif

#if USE_LIDAR
	lidarTransfer.begin(IFC_LIDAR_PORT);
#endif

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tTransfer classes initialized"));
#endif




	//initialize GPS
#if USE_GPS
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing GPS..."));
#endif

	gps.begin(IFC_GPS_PORT);

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tGPS initialized"));
#endif
#endif




	//initialize the IMU and wait for it to "boot up"
#if USE_IMU
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing IMU..."));
#endif

	if (!bno.begin())
	{
		//there was a problem detecting the BNO055 ... check your connections
#if USE_DEBUG
		IFC_DEBUG_PORT.println(F("\tOoops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
#endif

		//fatal error - halt code execution
		while (1);
	}
	else
	{
		delay(1000);
		bno.setExtCrystalUse(true);

#if USE_DEBUG
		IFC_DEBUG_PORT.println(F("\tBNO055 successfully initialized"));
#endif
	}
#endif




	//initialize each individual servo to their respective center position
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing servos..."));
#endif

	rudder.attach(RUDDER_PIN);
	elevator.attach(ELEVATOR_PIN);
	aileron.attach(AILERON_PIN);

	rudder.writeMicroseconds(RUDDER_MID);
	elevator.writeMicroseconds(ELEVATOR_MID);
	aileron.writeMicroseconds(AILERON_MID);

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tServos initialized..."));
#endif




	//initialize the ESC
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing ESC..."));
#endif

	throttle.attach(THROTTLE_PIN);
	throttle.write(THROTTLE_MIN);

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tESC initialized..."));
#endif




	//initialize the timers
#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initializing timers..."));
#endif

	lossLinkTimer.begin(LOSS_LINK_TIMEOUT);
	limiterTimer.begin(LIMITER_PERIOD);
	telemTimer.begin(REPORT_TELEM_PERIOD);

#if USE_GPS
	lossGPSTimer.begin(LOSS_GPS_TIMEOUT);
#endif

#if USE_IMU
	imuTimer.begin(LIMITER_PERIOD);
#endif

#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("\tTimers initialized..."));
#endif




#if USE_DEBUG
	IFC_DEBUG_PORT.println(F("Initialization complete"));
	IFC_DEBUG_PORT.println(F("--------------------------------------------------"));
#endif
}




bool IFC_Class::handleSerialEvents()
{
#if USE_LIDAR
	lidarEvent();
#endif

	return commEvent();
}




bool IFC_Class::commEvent()
{
	if (commandTransfer.available())
	{
		commandTransfer.rxObj(controlInputs);

		lossLinkTimer.start();
		linkConnected = true;
		telemetry.validFlags = telemetry.validFlags | 0x2;
	}

	return linkFailover();
}




//update servo positions (use controlInputs commands)
void IFC_Class::updateServos(const bool& overrideManEn)
{
	if (controlInputs.manual_control_enable || overrideManEn)
	{
		throttle.write(constrain(controlInputs.throttle_command, THROTTLE_MIN, THROTTLE_MAX));

		elevator.writeMicroseconds(constrain(controlInputs.pitch_command, ELEVATOR_MIN, ELEVATOR_MAX));
		rudder.writeMicroseconds(constrain(controlInputs.yaw_command,     RUDDER_MIN,   RUDDER_MAX));
		aileron.writeMicroseconds(constrain(controlInputs.roll_command,   AILERON_MIN,  AILERON_MAX));
	}
}




//update a single servo's position (use controlInputs commands)
void IFC_Class::updateSingleServo(const byte& INDEX, const uint16_t& value)
{
	if (INDEX == COMMAND_THROTTLE_INDEX)
		throttle.write(constrain(value, THROTTLE_MIN, THROTTLE_MAX));
	
	else if (INDEX == COMMAND_PITCH_INDEX)
		elevator.writeMicroseconds(constrain(value, ELEVATOR_MIN, ELEVATOR_MAX));
	
	else if (INDEX == COMMAND_YAW_INDEX)
		rudder.writeMicroseconds(constrain(value, RUDDER_MIN, RUDDER_MAX));
	
	else if (INDEX == COMMAND_ROLL_INDEX)
	{
		aileron.writeMicroseconds(constrain(value, AILERON_MIN, AILERON_MAX));
	}
	else
		IFC_DEBUG_PORT.println(F("Servo Not recognized"));
}




bool IFC_Class::linkFailover()
{
	if (lossLinkTimer.fire(false))
	{
		controlInputs.pitch_command = 1500;
		controlInputs.roll_command = 1500;
		controlInputs.yaw_command = 1500;
		controlInputs.throttle_command = 1500;

		updateServos();

		linkConnected = false;
		telemetry.validFlags = telemetry.validFlags & !0x2;
	}

	return linkConnected;
}
