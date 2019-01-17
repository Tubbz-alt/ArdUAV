#include "Arduino.h"




#ifndef Shared_Tools_cpp
#define Shared_Tools_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define DEBUG_PORT_BAUD       115200
#define COMMAND_PORT_BAUD     115200
#define GPS_PORT_BAUD         9600
#define TELEM_PORT_BAUD       9600

#define ANALOG_RESOLUTION     16

#define ELEVATOR_INDEX        0
#define AILERON_INDEX         1
#define RUDDER_INDEX          2
#define THROTTLE_INDEX        3
#define NOSE_GEAR_INDEX       5

#define THROTTLE_MAX          170 //full throttle
#define AILERON_MAX           320 //roll left
#define ELEVATOR_MAX          360 //nose up
#define RUDDER_MAX            330 //nose left

#define THROTTLE_MIN          50  //no throttle
#define AILERON_MIN           230 //roll right
#define ELEVATOR_MIN          220 //nose down
#define RUDDER_MIN            210 //nose right
/////////////////////////////////////////////////////////////////////////////////////////

#endif // !1