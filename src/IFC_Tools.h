#pragma once
#include "Arduino.h"
#include "Servo.h"
#include "FireTimer.h"
#include "IFC_Serial.h"
#include "Shared_Tools.h"
#include "SerialTransfer.h"
#include "neo6mGPS.h"
#include "ArdUAV_Adafruit_BNO055.h"




class IFC_Class : public base
{
public:
#if USE_IMU
	Adafruit_BNO055 bno = Adafruit_BNO055(&IMU_PORT, IMU_ID, BNO055_ADDRESS_A);
	void pollIMU();
	void bankPitchLimiter();
#endif

#if USE_GPS
	bool gpsConnected;
	neo6mGPS gps;
	bool pollGPS();
#endif

#if USE_PITOT
	void pollPitot();
#endif

#if USE_LIDAR
	SerialTransfer lidarTransfer;
#endif

	Servo rudder;
	Servo elevator;
	Servo aileron;
	Servo throttle;

	void begin();
	bool handleSerialEvents();
	bool commEvent();
	void updateServos(const bool& overrideManEn=false);
	void updateSingleServo(const byte& INDEX, const uint16_t& value);




private:
#if USE_IMU
	float convertedPitch;
	float convertedRoll;
	unsigned long dataTimestamp_IMU;
	FireTimer limiterTimer;
	FireTimer imuTimer;
	void updateControlsLimiter(const bool& axis);
#endif

#if USE_GPS
	FireTimer lossGPSTimer;
	bool gpsFailover();
#endif

#if USE_LIDAR
	byte LiDAR_Counter;
	void lidarEvent();
#endif

	bool linkFailover();
};
