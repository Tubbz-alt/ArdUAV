#pragma once
#include "Arduino.h"
#include "FireTimer.h"
#include "GS_Serial.h"
#include "Shared_Tools.h"
#include "SerialTransfer.h"




class GS_Class : public base
{
public:
#if USE_GS_TELEM
	SerialTransfer telemetryTransfer;

	void sendTelem();
#endif

	bool newTelem = false;
	control_inputs_struct controlInputs;

	void begin();
	bool tick();
	void computeAndSendCommands();
	void computeCommands();
	void sendCommands();




private:
	struct control_surfaces_struct ailerons
	{
		COMMAND_ROLL_INDEX,     //command_index
		ROLL_ANALOG_PIN,        //analog_pin
		AILERON_OFFSET,         //_offset
		AILERON_MAX,            //surface_max
		AILERON_MIN,            //surface_min
		AILERON_MAX_ADC,        //ADC_max
		AILERON_MIN_ADC,        //ADC_min
		AILERON_REVERSE,        //flag to reverse servo throw direction
	};

	struct control_surfaces_struct elevator
	{
		COMMAND_PITCH_INDEX,    //command_index
		PITCH_ANALOG_PIN,       //analog_pin
		ELEVATOR_OFFSET,        //_offset
		ELEVATOR_MAX,           //surface_max
		ELEVATOR_MIN,           //surface_min
		ELEVATOR_MAX_ADC,       //ADC_max
		ELEVATOR_MIN_ADC,       //ADC_min
		ELEVATOR_REVERSE,       //flag to reverse servo throw direction
	};

	struct control_surfaces_struct rudder
	{
		COMMAND_YAW_INDEX,      //command_index
		YAW_ANALOG_PIN,         //analog_pin
		RUDDER_OFFSET,          //_offset
		RUDDER_MAX,             //surface_max
		RUDDER_MIN,             //surface_min
		RUDDER_MAX_ADC,         //ADC_max
		RUDDER_MIN_ADC,         //ADC_max
		RUDDER_REVERSE,         //flag to reverse servo throw direction
	};

	struct control_surfaces_struct throttle
	{
		COMMAND_THROTTLE_INDEX, //command_index
		THROTTLE_ANALOG_PIN,    //analog_pin
		THROTTLE_OFFSET,        //_offset
		THROTTLE_MAX,           //surface_max
		THROTTLE_MIN,           //surface_min
		THROTTLE_MAX_ADC,       //ADC_max
		THROTTLE_MIN_ADC,       //ADC_max
		THROTTLE_REVERSE,       //flag to reverse motor direction
	};
	
	int16_t updateServoCommand(const control_surfaces_struct& controlSurface);
	bool linkFailover();
};
