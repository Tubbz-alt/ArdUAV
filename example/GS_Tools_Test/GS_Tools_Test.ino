#include "GS_Tools.h"




void setup()
{
  //initialize the core ground station software class
  myGS.begin();
}




void loop()
{
  //get telemetry data from the plane
  if(myGS.grabData_Radio())
  {
    //send data to datalogging computer via debugging port
    myGS.sendTelem();
  }

  //determine each command value based off GS sensor data and send commands to plane
  myGS.computeAndSendCommands();
}



