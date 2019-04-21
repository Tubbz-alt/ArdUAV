#include "Arduino.h"

#include "GS_Serial.h"
#include "Shared_Tools.h"
#include "AirComms.h"




#ifndef GS_Tools_cpp
#define GS_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define GS

#define YAW_ANALOG_PIN			A2
#define THROTTLE_ANALOG_PIN		A3
#define ROLL_ANALOG_PIN			A0
#define PITCH_ANALOG_PIN		A17

#define PITCH_RATE_PIN			0
#define ROLL_RATE_PIN			1
#define YAW_RATE_PIN			2

#define AILERON_MAX_LOWRATES        	2000
#define ELEVATOR_MAX_LOWRATES       	2000
#define RUDDER_MAX_LOWRATES         	2000

#define AILERON_MIN_LOWRATES       	1000
#define ELEVATOR_MIN_LOWRATES       	1000
#define RUDDER_MIN_LOWRATES         	1000

#define THROTTLE_MIN_ADC            	24130
#define AILERON_MIN_ADC             	25190
#define ELEVATOR_MIN_ADC            	24030
#define RUDDER_MIN_ADC              	25100

#define THROTTLE_MAX_ADC            	41060
#define AILERON_MAX_ADC             	41920
#define ELEVATOR_MAX_ADC            	41220
#define RUDDER_MAX_ADC              	41740
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//GS Class
class GS_Class
{
public:
	struct telemetry
	{
		float altitude;			//cm
		float rollAngle;		//radians
		float pitchAngle;		//radians
		float velocity;			//m/s
		float latitude;			//dd
		float longitude;		//dd
		uint16_t UTC_year;		//y
		uint16_t UTC_month;		//M
		uint16_t UTC_day;		//d
		uint16_t UTC_hour;		//h
		uint16_t UTC_minute;		//m
		uint16_t UTC_second;		//s
		float speedOverGround;		//knots
		float courseOverGround;		//degrees
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

	//send data to datalogging computer via debugging port
	void sendTelem();

	//determine each command value based off GS sensor data
	void computeCommands();

	//send commands to plane
	void sendCommands();

	//determine each command value based off GS sensor data and send commands to plane
	void computeAndSendCommands();




private:
	/////////////////////////////////////////////////////////////////////////////////////////
	//variables to implement "pass-through" timers
	unsigned long timeBench_Commands;
	unsigned long currentTime_Commands;

	unsigned long timeBench_Telem;
	unsigned long currentTime_Telem;
	/////////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////
	//structs to handle control surface commands
	struct controlSurfaces
	{
		byte command_index;
		byte analog_pin;
		byte rate_pin;
		uint16_t _offset;
		uint16_t high_rates_surface_max;
		uint16_t high_rates_surface_min;
		uint16_t low_rates_surface_max;
		uint16_t low_rates_surface_min;
		uint16_t ADC_max;
		uint16_t ADC_min;
	};

	struct controlSurfaces ailerons
	{
		AIR_ROLL_INDEX,         //command_index
		ROLL_ANALOG_PIN,        //analog_pin
		ROLL_RATE_PIN,          //rate_pin
		AILERON_OFFSET,         //_offset
		AILERON_MAX,		//high_rates_surface_max
		AILERON_MIN,		//high_rates_surface_min
		AILERON_MAX_LOWRATES,   //low_rates_surface_max
		AILERON_MIN_LOWRATES,   //low_rates_surface_min
		AILERON_MAX_ADC,        //ADC_max
		AILERON_MIN_ADC         //ADC_min
	};

	struct controlSurfaces elevator
	{
		AIR_PITCH_INDEX,        //command_index
		PITCH_ANALOG_PIN,       //analog_pin
		PITCH_RATE_PIN,         //rate_pin
		ELEVATOR_OFFSET,        //_offset
		ELEVATOR_MAX,		//high_rates_surface_max
		ELEVATOR_MIN,		//high_rates_surface_min
		ELEVATOR_MAX_LOWRATES,  //low_rates_surface_max
		ELEVATOR_MIN_LOWRATES,  //low_rates_surface_min
		ELEVATOR_MAX_ADC,       //ADC_max
		ELEVATOR_MIN_ADC        //ADC_min
	};

	struct controlSurfaces rudder
	{
		AIR_YAW_INDEX,          //command_index
		YAW_ANALOG_PIN,         //analog_pin
		YAW_RATE_PIN,           //rate_pin
		RUDDER_OFFSET,          //_offset
		RUDDER_MAX,		//high_rates_surface_max
		RUDDER_MIN,		//high_rates_surface_min
		RUDDER_MAX_LOWRATES,    //low_rates_surface_max
		RUDDER_MIN_LOWRATES,    //low_rates_surface_min
		RUDDER_MAX_ADC,         //ADC_max
		RUDDER_MIN_ADC          //ADC_max
	};

	struct controlSurfaces throttle
	{
		AIR_THROTTLE_INDEX,     //command_index
		THROTTLE_ANALOG_PIN,    //analog_pin
		0,                      //rate_pin
		THROTTLE_OFFSET,        //_offset
		THROTTLE_MAX,           //high_rates_surface_max
		THROTTLE_MIN,           //high_rates_surface_min
		0,                      //low_rates_surface_max
		0,                      //low_rates_surface_min
		THROTTLE_MAX_ADC,       //ADC_max
		THROTTLE_MIN_ADC        //ADC_max
	};
	/////////////////////////////////////////////////////////////////////////////////////////




	//calculate the values for a single control surface servo command
	int16_t updateServoCommand(controlSurfaces controlSurface);

	//check to see if there is a loss of radio link between GS and IFC
	bool checkRadioLink();
};

extern GS_Class myGS;
/////////////////////////////////////////////////////////////////////////////////////////

#endif
