#include "Arduino.h"

#include "Shared_Tools.h"
#include "IFC_AirComms.h"




#ifndef GS_Tools_cpp
#define GS_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define GS

#define DEBUG_PORT_NUMBER     0	//Serial
#define COMMAND_PORT_NUMBER   4	//Serial4
#define TELEM_PORT_NUMBER     5	//Serial5

#define YAW_ANALOG_PIN        0
#define THROTTLE_ANALOG_PIN   1
#define ROLL_ANALOG_PIN       2
#define PITCH_ANALOG_PIN	  3

#define PITCH_RATE_PIN        0
#define ROLL_RATE_PIN         1
#define YAW_RATE_PIN          2
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//GS Class
class GS_Class
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

	//get data from GS
	int grabData_Radio();

	//send telemetry data to GS
	void sendCommands();
};

extern GS_Class myGS;
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
#elif COMMAND_PORT_NUMBER == 1
#define COMMAND_PORT Serial1
#elif COMMAND_PORT_NUMBER == 2
#define COMMAND_PORT Serial2
#elif COMMAND_PORT_NUMBER == 3
#define COMMAND_PORT Serial3
#elif COMMAND_PORT_NUMBER == 4
#define COMMAND_PORT Serial4
#elif COMMAND_PORT_NUMBER == 5
#define COMMAND_PORT Serial5
#elif COMMAND_PORT_NUMBER == 6
#define COMMAND_PORT Serial6
#endif




#if TELEM_PORT_NUMBER == 0

#define TELEM_PORT Serial
extern void serialEvent();

#elif TELEM_PORT_NUMBER == 1

#define TELEM_PORT Serial1
extern void serialEvent1();

#elif TELEM_PORT_NUMBER == 2

#define TELEM_PORT Serial2
extern void serialEvent2();

#elif TELEM_PORT_NUMBER == 3

#define TELEM_PORT Serial3
extern void serialEvent3();

#elif TELEM_PORT_NUMBER == 4

#define TELEM_PORT Serial4
extern void serialEvent4();

#elif TELEM_PORT_NUMBER == 5

#define TELEM_PORT Serial5
extern void serialEvent5();

#elif TELEM_PORT_NUMBER == 6

#define TELEM_PORT Serial6
extern void serialEvent6();

#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////