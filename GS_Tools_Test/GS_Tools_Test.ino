#include "GS_Tools.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
//setup user defined global variables and functions here
//below are variables to make a "pass-through timer" allowing the plane to periodically send data
//to the plane at a fixed rate without blocking processing - default is 10ms = 100Hz
unsigned long timeBench = millis();
unsigned long currentTime = timeBench;
byte transmitPeriod = 10;
///////////////////////////////////////////////////////////////////////////////////////////////////




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
    //optinal debugging prints
    /*GS_DEBUG_PORT.print("Altitude: "); GS_DEBUG_PORT.println(myGS.telemetry.altitude, 2);
    GS_DEBUG_PORT.print("Roll Angle: "); GS_DEBUG_PORT.println(myGS.telemetry.rollAngle, 2);
    GS_DEBUG_PORT.print("Pitch Angle: "); GS_DEBUG_PORT.println(myGS.telemetry.pitchAngle, 2);
    GS_DEBUG_PORT.print("Velocity: "); GS_DEBUG_PORT.println(myGS.telemetry.velocity, 2);
    GS_DEBUG_PORT.print("Latitude: "); GS_DEBUG_PORT.println(myGS.telemetry.latitude, 2);
    GS_DEBUG_PORT.print("Longitude: "); GS_DEBUG_PORT.println(myGS.telemetry.longitude, 2);
    GS_DEBUG_PORT.println();*/
  }

  //use timer to send commands to the plane at a fixed rate
  currentTime = millis();
  if((currentTime - timeBench)>= transmitPeriod)
  {
    timeBench = currentTime;

    //update command values
    myGS.computeCommands();

    //send commands to the plane
    myGS.sendCommands();
  }
}




