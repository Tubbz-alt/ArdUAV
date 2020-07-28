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
	Servo rudder;
	Servo elevator;
	Servo aileron;
	Servo throttle;
	neo6mGPS gps;
	SerialTransfer lidarTransfer;
	Adafruit_BNO055 bno = Adafruit_BNO055(&Wire, 55, BNO055_ADDRESS_A);

	void begin();
	bool handleSerialEvents();
	bool pollGPS();
	void pollIMU();
	void pollPitot();
	void sendTelem();
	void updateServos(const bool& overrideManEn=false);
	void updateSingleServo(const byte& INDEX, const uint16_t& value);
	void bankPitchLimiter();




private:
	float convertedPitch;
	float convertedRoll;
	byte LiDAR_Counter;
	unsigned long dataTimestamp_IMU;
	FireTimer limiterTimer;
	FireTimer imuTimer;

	bool linkFailover();
	bool commEvent();
	void lidarEvent();
	void updateControlsLimiter(const bool& axis);
};

extern TwoWire Wire1;
extern TwoWire Wire2;
