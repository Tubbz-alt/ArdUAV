#include "Arduino.h"
#include "Servo.h"

#include "IFC_Serial.h"
#include "Shared_Tools.h"
#include "AirComms.h"
#include "neo6mGPS.h"
#include "ArdUAV_Adafruit_BNO055.h"
#include "ArdUAV_LIDARLite.h"




#ifndef IFC_Tools_cpp
#define IFC_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define IFC

#define LIDAR_FIXED_MOUNT		1	//0 - gimbal mount, 1 - fixed mount

#define SERVO_FREQ			60	//Hz
#define LIMITER_PERIOD			REPORT_COMMANDS_PERIOD //ms

#define PITOT_PIN			A9	//analog input pin

#define THROTTLE_PIN			17	//digital ESC signal pin
#define R_AILERON_PIN			2	//servo driver output port number
#define L_AILERON_PIN			39	//servo driver output port number
#define ELEVATOR_PIN			14	//servo driver output port number
#define RUDDER_PIN			25	//servo driver output port number

#define PITCH_AXIS			true	//
#define ROLL_AXIS			false	//

#define UNSAFE_ROLL_R			-35	//unsafe right bank angle threshold (in degrees)
#define UNSAFE_ROLL_L			35	//unsafe left bank angle threshold (in degrees)

#define MAX_ROLL_R			-50	//max right bank angle (in degrees) allowed by flight controller
#define MAX_ROLL_L			50	//max left bank angle (in degrees) allowed by flight controller

#define UNSAFE_PITCH_UP			-30	//unsafe up pitch angle threshold (in degrees)
#define UNSAFE_PITCH_DOWN		10	//unsafe down pitch angle threshold (in degrees)

#define MAX_PITCH_UP			-45	//max up pitch angle (in degrees) allowed by flight controller
#define MAX_PITCH_DOWN			25	//max down pitch angle (in degrees) allowed by flight controller
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//IFC Class
class IFC_Class
{
public:
	//struct to store telemetry data
	struct telemetry
	{
		float altitude;				//cm
		float convertedAltitude;	//cm
		float courseAngle;			//degrees
		float rollAngle;			//degrees
		float pitchAngle;			//degrees
		float convertedRoll;		//radians
		float convertedPitch;		//radians
		float velocity;				//m/s
		float latitude;				//dd
		float longitude;			//dd
		uint16_t UTC_year;			//y
		uint16_t UTC_month;			//M
		uint16_t UTC_day;			//d
		uint16_t UTC_hour;			//h
		uint16_t UTC_minute;		//m
		uint16_t UTC_second;		//s
		float speedOverGround;		//knots
		float courseOverGround;		//degrees
	} telemetry;

	//struct to store control values (i.e. servo commands etc.)
	struct controlInputs
	{
		bool limiter_enable;		//enables and disables pitch and bank limiter
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

	//get airpseed data from pitot tube
	int grabData_Pitot();

	//send telemetry data to GS
	void sendTelem();

	//update servo positions (use controlInputs commands)
	void updateServos();

	//update a single servo's position (use controlInputs commands)
	void updateSingleServo(byte INDEX, uint16_t value);




private:
	//byte used to determine if LiDAR reading needs to include bias correction
	byte LiDAR_Counter;

	/////////////////////////////////////////////////////////////////////////////////////////
	//variables to implement "pass-through" timers
	unsigned long timeBench_Limiter;
	unsigned long currentTime_Limiter;

	unsigned long timeBench_Commands;
	unsigned long currentTime_Commands;

	unsigned long timeBench_Telem;
	unsigned long currentTime_Telem;
	/////////////////////////////////////////////////////////////////////////////////////////

	//IMU data timestamp
	unsigned long dataTimestamp_IMU;



	//check to see if there is a loss of radio link between GS and IFC
	bool checkRadioLink();

	//keep the plane from pitching or rolling too much in any direction
	void bankPitchLimiter(bool enable, bool _linkConnected);

	//update struct based on euler angles
	void updateControlsLimiter(bool axis);
};

extern IFC_Class myIFC;

extern TwoWire Wire1;
extern TwoWire Wire2;
/////////////////////////////////////////////////////////////////////////////////////////

#endif
