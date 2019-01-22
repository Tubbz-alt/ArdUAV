#include "IFC_Tools.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
//setup user defined global variables and functions here
//below are variables to make a "pass-through timer" allowing the plane to periodically send telem
//to the ground station at a fixed rate without blocking processing - default is 100ms = 10Hz
unsigned long timeBench = millis();
unsigned long currentTime = timeBench;
byte transmitPeriod = 100;
///////////////////////////////////////////////////////////////////////////////////////////////////




void setup()
{
  //initialize the core flight controller software class
  myIFC.begin();
}




void loop()
{
  //get GPS data
  if(myIFC.grabData_GPS())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Latitude:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.latitude, 5);
    IFC_DEBUG_PORT.print("Longitude:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.longitude, 5);
    IFC_DEBUG_PORT.println();*/
  }

  //get IMU data
  if(myIFC.grabData_IMU())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Pitch Angle:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.pitchAngle, 5);
    IFC_DEBUG_PORT.print("Roll Angle:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.rollAngle, 5);
    IFC_DEBUG_PORT.println();*/
  }

  //get LiDAR altimeter data
  if(myIFC.grabData_LiDAR())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Converted Altitude:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.convertedAltitude);
    IFC_DEBUG_PORT.println();*/
  }

  //get data from the ground station
  if(myIFC.grabData_Radio())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Pitch Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.pitch_command);
    IFC_DEBUG_PORT.print("Roll Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.roll_command);
    IFC_DEBUG_PORT.print("Yaw Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.yaw_command);
    IFC_DEBUG_PORT.print("Throttle Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.throttle_command);
    IFC_DEBUG_PORT.print("Autopilot Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.autopilot_command);
    IFC_DEBUG_PORT.print("Limiter Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.limiter_command);
    IFC_DEBUG_PORT.print("Landing Gear Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.landingGear_command);
    IFC_DEBUG_PORT.print("Flaps Command:\t"); IFC_DEBUG_PORT.println(myIFC.controlInputs.flaps_command);
    IFC_DEBUG_PORT.println();*/

    //update all servo positions based on the new data
    myIFC.updateServos();
  }

  //use timer to send telemetry data to the ground station at a fixed rate
  currentTime = millis();
  if((currentTime - timeBench)>= transmitPeriod)
  {
    timeBench = currentTime;

    //send the telemetry to the radio
    myIFC.sendTelem();
  }
}




