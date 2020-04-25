#pragma once
#include "Arduino.h"
#include "Servo.h"
#include "FireTimer.h"

#include "IFC_Serial.h"
#include "Shared_Tools.h"
#include "SerialTransfer.h"
#include "neo6mGPS.h"
#include "ArdUAV_Adafruit_BNO055.h"




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define IFC

#define LIDAR_FIXED_MOUNT  0  //0 - gimbal mount, 0 - fixed mount

#define SERVO_FREQ         60 //Hz
#define LIMITER_PERIOD     REPORT_COMMANDS_PERIOD //ms

#define PITOT_PIN          A17 //analog input pin

#define THROTTLE_PIN       2 //digital ESC signal pin
#define AILERON_PIN        39 //servo driver output port number
#define ELEVATOR_PIN       24 //servo driver output port number
#define RUDDER_PIN         6 //servo driver output port number

#define PITCH_AXIS         true  //
#define ROLL_AXIS          false //

#define UNSAFE_ROLL_R      -35 //unsafe right bank angle threshold (in degrees)
#define UNSAFE_ROLL_L      35  //unsafe left bank angle threshold (in degrees)

#define MAX_ROLL_R         -50 //max right bank angle (in degrees) allowed by flight controller
#define MAX_ROLL_L         50  //max left bank angle (in degrees) allowed by flight controller

#define UNSAFE_PITCH_UP    -30 //unsafe up pitch angle threshold (in degrees)
#define UNSAFE_PITCH_DOWN  10  //unsafe down pitch angle threshold (in degrees)

#define MAX_PITCH_UP       -45 //max up pitch angle (in degrees) allowed by flight controller
#define MAX_PITCH_DOWN     25  //max down pitch angle (in degrees) allowed by flight controller
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//IFC Class
class IFC_Class
{
public:
	//struct to store telemetry data
	struct telemetry
	{
		float altitude;             //cm
		float convertedAltitude;    //cm
		float courseAngle;          //degrees
		float rollAngle;            //degrees
		float pitchAngle;           //degrees
		float convertedRoll;        //radians
		float convertedPitch;       //radians
		float velocity;             //m/s
		float latitude;             //dd
		float longitude;            //dd
		uint16_t UTC_year;          //y
		uint8_t UTC_month;          //M
		uint8_t UTC_day;            //d
		uint8_t UTC_hour;           //h
		uint8_t UTC_minute;         //m
		float UTC_second;           //s
		float speedOverGround;      //knots
		float courseOverGround;     //degrees
	} telemetry;

	//struct to store control values (i.e. servo commands etc.)
	struct controlInputs
	{
		bool limiter_enable; //enables and disables pitch and bank limiter
		uint16_t pitch_command;
		uint16_t roll_command;
		uint16_t yaw_command;
		uint16_t throttle_command;
		uint16_t autopilot_command;
		uint16_t limiter_command;
		uint16_t gear_command;
		uint16_t flaps_command;
	} controlInputs;




	void begin();
	bool grabData_GPS();
	int grabData_IMU();
	int grabData_Pitot();
	void sendTelem();
	void updateServos();
	void updateSingleServo(byte INDEX, uint16_t value);
	void bankPitchLimiter(bool enable, bool _linkConnected);




private:
	//byte used to determine if LiDAR reading needs to include bias correction
	byte LiDAR_Counter;
	unsigned long dataTimestamp_IMU;
	FireTimer limiterTimer;
	FireTimer telemTimer;
	FireTimer imuTimer;




	void updateControlsLimiter(bool axis);
};

extern IFC_Class myIFC;
extern neo6mGPS myGPS;
extern SerialTransfer IFC_commandTransfer;
extern SerialTransfer IFC_telemetryTransfer;
extern SerialTransfer IFC_lidarTransfer;

extern TwoWire Wire1;
extern TwoWire Wire2;
/////////////////////////////////////////////////////////////////////////////////////////
