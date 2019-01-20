#include "Arduino.h"
#include "Servo.h"

#include "IFC_Serial.h"
#include "Shared_Tools.h"
#include "AirComms.h"
#include "neo6mGPS.h"
#include "ArdUAV_Adafruit_BNO055.h"
#include "ArdUAV_Adafruit_PWMServoDriver.h"
#include "ArdUAV_LIDARLite.h"




#ifndef IFC_Tools_cpp
#define IFC_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define IFC

#define LIDAR_FIXED_MOUNT     1

#define REPORT_PERIOD         100

#define PITOT_PIN             A9

#define THROTTLE_PIN          28
#define NOSE_GEAR_PIN         4
#define R_AILERON_PIN         3
#define L_AILERON_PIN         2
#define ELEVATOR_PIN          1
#define RUDDER_PIN            0

#define MAXPITCHUP            -20
#define UNSAFEPITCHUP         -35

#define MAXPITCHDOWN          10
#define UNSAFEPITCHDOWN       30

#define MAXROLL_RIGHT         -35
#define UNSAFEROLL_RIGHT      -45
#define MAXROLL_LEFT          35
#define UNSAFEROLL_LEFT       45

#define PITCH_CORRECTION_MIN  0
#define PITCH_CORRECTION_MAX  30

#define ROLL_CORRECTION_MIN   0
#define ROLL_CORRECTION_MAX   10

#define SERVO_MIN             0
#define SERVO_MAX             0
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//sensor variables
extern byte LiDAR_Counter;
extern unsigned long startTime;
extern unsigned long endTime;
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//IFC Class
class IFC_Class
{
public:
	struct telemetry
	{
		float altitude;
		float convertedAltitude;
		float rollAngle;
		float pitchAngle;
		float convertedRoll;
		float convertedPitch;
		float velocity;
		float latitude;
		float longitude;
	} telemetry;

	struct controlInputs
	{
		uint16_t pitch_command;
		uint16_t roll_command;
		uint16_t yaw_command;
		uint16_t throttle_command;
		uint16_t autopilot_command;
		uint16_t limiter_command;
		uint16_t landingGear_command;
		uint16_t flaps_command;
	} controlInputs;

	//initialize the IFC class
	void begin();

	//get GPS data
	int grabData_GPS();

	//get IMU Data
	int grabData_IMU();

	//get LiDAR Altimeter Data
	int grabData_LiDAR();

	//get data from GS
	int grabData_Radio();

	//send telemetry data to GS
	void sendTelem();

	//update servo positions (use controlInputs commands)
	void updateServos();

	//update a single servo's position (use controlInputs commands)
	void updateSingleServo(byte INDEX, uint16_t value);
};

extern IFC_Class myIFC;
/////////////////////////////////////////////////////////////////////////////////////////

#endif