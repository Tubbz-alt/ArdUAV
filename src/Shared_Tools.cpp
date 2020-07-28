#include "Shared_Tools.h"
#include "SerialTransfer.h"




//shared variables/flags
bool linkConnected = false; //false if radio link between GS and IFC is lost
bool packetDetected = false; //true if packet is detected




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
