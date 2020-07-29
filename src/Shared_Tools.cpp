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
