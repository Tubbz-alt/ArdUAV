#include "Arduino.h"

#include "Shared_Tools.h"




#ifndef GS_Serial_cpp
#define GS_Serial_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define GS_DEBUG_PORT_NUMBER      0	//Serial
#define GS_COMMAND_PORT_NUMBER    4	//Serial4
#define GS_TELEM_PORT_NUMBER      5	//Serial5
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if GS_DEBUGD_PORT_NUMBER == 0
#define GS_DEBUG_PORT Serial
#elif GS_DEBUG_PORT_NUMBER == 1
#define GS_DEBUG_PORT Serial1
#elif GS_DEBUG_PORT_NUMBER == 2
#define GS_DEBUG_PORT Serial2
#elif GS_DEBUG_PORT_NUMBER == 3
#define GS_DEBUG_PORT Serial3
#elif GS_DEBUG_PORT_NUMBER == 4
#define GS_DEBUG_PORT Serial4
#elif GS_DEBUG_PORT_NUMBER == 5
#define GS_DEBUG_PORT Serial5
#elif GS_DEBUG_PORT_NUMBER == 6
#define GS_DEBUG_PORT Serial6
#endif




#if GS_COMMAND_PORT_NUMBER == 0
#define GS_COMMAND_PORT Serial
#elif GS_COMMAND_PORT_NUMBER == 1
#define GS_COMMAND_PORT Serial1
#elif GS_COMMAND_PORT_NUMBER == 2
#define GS_COMMAND_PORT Serial2
#elif GS_COMMAND_PORT_NUMBER == 3
#define GS_COMMAND_PORT Serial3
#elif GS_COMMAND_PORT_NUMBER == 4
#define GS_COMMAND_PORT Serial4
#elif GS_COMMAND_PORT_NUMBER == 5
#define GS_COMMAND_PORT Serial5
#elif GS_COMMAND_PORT_NUMBER == 6
#define GS_COMMAND_PORT Serial6
#endif




#if GS_TELEM_PORT_NUMBER == 0

#define GS_TELEM_PORT Serial
extern void serialEvent();

#elif GS_TELEM_PORT_NUMBER == 1

#define GS_TELEM_PORT Serial1
extern void serialEvent1();

#elif GS_TELEM_PORT_NUMBER == 2

#define GS_TELEM_PORT Serial2
extern void serialEvent2();

#elif GS_TELEM_PORT_NUMBER == 3

#define GS_TELEM_PORT Serial3
extern void serialEvent3();

#elif GS_TELEM_PORT_NUMBER == 4

#define GS_TELEM_PORT Serial4
extern void serialEvent4();

#elif GS_TELEM_PORT_NUMBER == 5

#define GS_TELEM_PORT Serial5
extern void serialEvent5();

#elif GS_TELEM_PORT_NUMBER == 6

#define GS_TELEM_PORT Serial6
extern void serialEvent6();

#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////

#endif