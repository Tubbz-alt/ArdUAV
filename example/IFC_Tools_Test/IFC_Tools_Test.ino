#include "IFC_Tools.h"




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
    IFC_DEBUG_PORT.print("UTC_year:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_year);
    IFC_DEBUG_PORT.print("UTC_month:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_month);
    IFC_DEBUG_PORT.print("UTC_day:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_day);
    IFC_DEBUG_PORT.print("UTC_hour:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_hour);
    IFC_DEBUG_PORT.print("UTC_minute:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_minute);
    IFC_DEBUG_PORT.print("UTC_second:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.UTC_second);
    IFC_DEBUG_PORT.print("Speed Over Ground:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.speedOverGround, 3);
    IFC_DEBUG_PORT.print("Course Over Ground:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.courseOverGround, 3);
    IFC_DEBUG_PORT.println();*/
  }

  //get IMU data
  if(myIFC.grabData_IMU())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Pitch Angle:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.pitchAngle, 5);
    IFC_DEBUG_PORT.print("Roll Angle:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.rollAngle, 5);
    //IFC_DEBUG_PORT.print("Course Angle:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.courseAngle, 5);
    IFC_DEBUG_PORT.println();*/
  }

  //get LiDAR altimeter data
  if(myIFC.grabData_LiDAR())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Converted Altitude:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.convertedAltitude);
    IFC_DEBUG_PORT.println();*/
  }

  //get airspeed data from the pitot tube
  if(myIFC.grabData_Pitot())
  {
    //optional debugging prints
    /*IFC_DEBUG_PORT.print("Converted Airspeed:\t"); IFC_DEBUG_PORT.println(myIFC.telemetry.velocity);
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

  //send the telemetry to the radio
  myIFC.sendTelem();
}




