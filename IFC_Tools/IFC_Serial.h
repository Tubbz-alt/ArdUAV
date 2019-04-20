#include "Arduino.h"




#ifndef IFC_Serial_cpp
#define IFC_Serial_cpp




/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define IFC_DEBUG_PORT_NUMBER     0	//USB
#define IFC_COMMAND_PORT_NUMBER   4	//Serial4
#define IFC_GPS_PORT_NUMBER       1	//Serial1
#define IFC_TELEM_PORT_NUMBER     3	//Serial3
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
#if IFC_DEBUGD_PORT_NUMBER == 0
#define IFC_DEBUG_PORT Serial
#elif IFC_DEBUG_PORT_NUMBER == 1
#define IFC_DEBUG_PORT Serial1
#elif IFC_DEBUG_PORT_NUMBER == 2
#define IFC_DEBUG_PORT Serial2
#elif IFC_DEBUG_PORT_NUMBER == 3
#define IFC_DEBUG_PORT Serial3
#elif IFC_DEBUG_PORT_NUMBER == 4
#define IFC_DEBUG_PORT Serial4
#elif IFC_DEBUG_PORT_NUMBER == 5
#define IFC_DEBUG_PORT Serial5
#elif IFC_DEBUG_PORT_NUMBER == 6
#define IFC_DEBUG_PORT Serial6
#endif




#if IFC_COMMAND_PORT_NUMBER == 0

#define IFC_COMMAND_PORT Serial
extern void serialEvent();

#elif IFC_COMMAND_PORT_NUMBER == 1

#define IFC_COMMAND_PORT Serial1
extern void serialEvent1();

#elif IFC_COMMAND_PORT_NUMBER == 2

#define IFC_COMMAND_PORT Serial2
extern void serialEvent2();

#elif IFC_COMMAND_PORT_NUMBER == 3

#define IFC_COMMAND_PORT Serial3
extern void serialEvent3();

#elif IFC_COMMAND_PORT_NUMBER == 4

#define IFC_COMMAND_PORT Serial4
extern void serialEvent4();

#elif IFC_COMMAND_PORT_NUMBER == 5

#define IFC_COMMAND_PORT Serial5
extern void serialEvent5();

#elif IFC_COMMAND_PORT_NUMBER == 6

#define IFC_COMMAND_PORT Serial6
extern void serialEvent6();

#endif




#if IFC_GPS_PORT_NUMBER == 0

#define IFC_GPS_PORT Serial
extern void serialEvent();

#elif IFC_GPS_PORT_NUMBER == 1

#define IFC_GPS_PORT Serial1
extern void serialEvent1();

#elif IFC_GPS_PORT_NUMBER == 2

#define IFC_GPS_PORT Serial2
extern void serialEvent2();

#elif IFC_GPS_PORT_NUMBER == 3

#define IFC_GPS_PORT Serial3
extern void serialEvent3();

#elif IFC_GPS_PORT_NUMBER == 4

#define IFC_GPS_PORT Serial4
extern void serialEvent4();

#elif IFC_GPS_PORT_NUMBER == 5

#define IFC_GPS_PORT Serial5
extern void serialEvent5();

#elif IFC_GPS_PORT_NUMBER == 6

#define IFC_GPS_PORT Serial6
extern void serialEvent6();

#endif




#if IFC_TELEM_PORT_NUMBER == 0
#define IFC_TELEM_PORT Serial
#elif IFC_TELEM_PORT_NUMBER == 1
#define IFC_TELEM_PORT Serial1
#elif IFC_TELEM_PORT_NUMBER == 2
#define IFC_TELEM_PORT Serial2
#elif IFC_TELEM_PORT_NUMBER == 3
#define IFC_TELEM_PORT Serial3
#elif IFC_TELEM_PORT_NUMBER == 4
#define IFC_TELEM_PORT Serial4
#elif IFC_TELEM_PORT_NUMBER == 5
#define IFC_TELEM_PORT Serial5
#elif IFC_TELEM_PORT_NUMBER == 6
#define IFC_TELEM_PORT Serial6
#endif
//DO NOT EDIT THIS BLOCK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////

#endif // !1
