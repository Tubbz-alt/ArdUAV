#include "Arduino.h"
#include "Servo.h"

#include "Shared_Tools.h"
#include "IFC_neo6mGPS.h"
#include "IFC_Adafruit_BNO055.h"
#include "IFC_Adafruit_PWMServoDriver.h"
#include "IFC_LIDARLite.h"
#include "IFC_AirComms.h"




#ifndef IFC_Tools_cpp
#define IFC_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define IFC

#define DEBUG_PORT_NUMBER     0	//Serial
#define COMMAND_PORT_NUMBER   2	//Serial2
#define GPS_PORT_NUMBER       3	//Serial3
#define TELEM_PORT_NUMBER     4	//Serial4

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




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if DEBUGD_PORT_NUMBER == 0
#define DEBUG_PORT Serial
#elif DEBUG_PORT_NUMBER == 1
#define DEBUG_PORT Serial1
#elif DEBUG_PORT_NUMBER == 2
#define DEBUG_PORT Serial2
#elif DEBUG_PORT_NUMBER == 3
#define DEBUG_PORT Serial3
#elif DEBUG_PORT_NUMBER == 4
#define DEBUG_PORT Serial4
#elif DEBUG_PORT_NUMBER == 5
#define DEBUG_PORT Serial5
#elif DEBUG_PORT_NUMBER == 6
#define DEBUG_PORT Serial6
#endif




#if COMMAND_PORT_NUMBER == 0

#define COMMAND_PORT Serial
extern void serialEvent();

#elif COMMAND_PORT_NUMBER == 1

#define COMMAND_PORT Serial1
extern void serialEvent1();

#elif COMMAND_PORT_NUMBER == 2

#define COMMAND_PORT Serial2
extern void serialEvent2();

#elif COMMAND_PORT_NUMBER == 3

#define COMMAND_PORT Serial3
extern void serialEvent3();

#elif COMMAND_PORT_NUMBER == 4

#define COMMAND_PORT Serial4
extern void serialEvent4();

#elif COMMAND_PORT_NUMBER == 5

#define COMMAND_PORT Serial5
extern void serialEvent5();

#elif COMMAND_PORT_NUMBER == 6

#define COMMAND_PORT Serial6
extern void serialEvent6();

#endif




#if GPS_PORT_NUMBER == 0

#define GPS_PORT Serial
extern void serialEvent();

#elif GPS_PORT_NUMBER == 1

#define COMMAND_PORT Serial1
extern void serialEvent1();

#elif GPS_PORT_NUMBER == 2

#define GPS_PORT Serial2
extern void serialEvent2();

#elif GPS_PORT_NUMBER == 3

#define GPS_PORT Serial3
extern void serialEvent3();

#elif GPS_PORT_NUMBER == 4

#define GPS_PORT Serial4
extern void serialEvent4();

#elif GPS_PORT_NUMBER == 5

#define GPS_PORT Serial5
extern void serialEvent5();

#elif GPS_PORT_NUMBER == 6

#define GPS_PORT Serial6
extern void serialEvent6();

#endif




#if TELEM_PORT_NUMBER == 0
#define TELEM_PORT Serial
#elif TELEM_PORT_NUMBER == 1
#define TELEM_PORT Serial1
#elif TELEM_PORT_NUMBER == 2
#define TELEM_PORT Serial2
#elif TELEM_PORT_NUMBER == 3
#define TELEM_PORT Serial3
#elif TELEM_PORT_NUMBER == 4
#define TELEM_PORT Serial4
#elif TELEM_PORT_NUMBER == 5
#define TELEM_PORT Serial5
#elif TELEM_PORT_NUMBER == 6
#define TELEM_PORT Serial6
#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////