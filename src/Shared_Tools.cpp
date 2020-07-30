#include "Shared_Tools.h"
#include "SerialTransfer.h"




#if USE_TELEM
void base::sendTelem(SerialTransfer connection)
{
	if (telemTimer.fire())
	{
		uint16_t sendLen = connection.txObj(telemetry);
		sendLen += connection.txObj(controlInputs, sendLen);
		sendLen += TELEMETRY_BUFFER;

		connection.sendData(sendLen);
	}
}
#endif




float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
