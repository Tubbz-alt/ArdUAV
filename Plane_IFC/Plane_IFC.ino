#include <Wire.h>
#include <AirComms.h>
#include <neo6mGPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_PWMServoDriver.h>
#include <LIDARLite.h>
#include <Servo.h>



/////////////////////////////////////////////////////////////////////////////////////////
//macros
#define M_PI          3.141593

#define PITOT_PIN     0

#define THROTTLE_PIN  28
#define NOSE_GEAR_PIN 4
#define R_AILERON_PIN 3
#define L_AILERON_PIN 2
#define ELEVATOR_PIN  1
#define RUDDER_PIN    0

#define THROTTLE_MAX  170 //full throttle
#define AILERON_MAX   320 //roll left
#define ELEVATOR_MAX  310 //nose up
#define RUDDER_MAX    340 //nose left

#define THROTTLE_MIN  50  //no throttle
#define AILERON_MIN   230 //roll right
#define ELEVATOR_MIN  230 //nose down
#define RUDDER_MIN    220 //nose right

#define MAXPITCHUP    -20
#define UNSAFEPITCHUP -35

#define MAXPITCHDOWN    10
#define UNSAFEPITCHDOWN 30

#define MAXROLL_RIGHT     -35
#define UNSAFEROLL_RIGHT  -45
#define MAXROLL_LEFT      35
#define UNSAFEROLL_LEFT   45

#define PITCH_CORRECTION_MIN 0
#define PITCH_CORRECTION_MAX 30

#define ROLL_CORRECTION_MIN 0
#define ROLL_CORRECTION_MAX 10

#define SERVO_MIN 0
#define SERVO_MAX 0
/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
//class initializations
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
LIDARLite myLidarLite;
Servo throttle;
neo6mGPS myGPS;
AirComms commandsRadio;
AirComms telemetryRadio;

/////////////////////////////////////////////////////////////////////////////////////////
/*
 * Command Array Anatomy:
 * 
 * --------------------------------------------------------------------------------------
 * Index Number - Command Type                    - Command Format
 * --------------------------------------------------------------------------------------
 *      0       - pitch                           - 16-Bit Analog
 *      1       - roll                            - 16-Bit Analog
 *      2       - yaw                             - 16-Bit Analog
 *      3       - throttle                        - 16-Bit Analog
 *      4       - Autopilot Toggle (MSB)          - 8-bit  Boolean
 *              - Pitch/Roll Limiter Toggle (LSB) - 8-bit  Boolean
 *      5       - Landing Gear Toggle (MSB)       - 8-bit  Boolean
 *              - Flaps Toggle (LSB)              - 8-bit  Boolean
 *      6       - Unused                          - Unused
 *      7       - Unused                          - Unused
 *      8       - Unused                          - Unused
 *      9       - Unused                          - Unused
 *      10      - Unused                          - Unused
 *      11      - Unused                          - Unused
 *      12      - Unused                          - Unused
 *      13      - Unused                          - Unused
 *      14      - Unused                          - Unused
 *      15      - Unused                          - Unused
 *      16      - Unused                          - Unused
 *      17      - Unused                          - Unused 
 *      18      - Unused                          - Unused
 *      19      - Unused                          - Unused
 */
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/*
 * Telemetry Array Anatomy:
 * 
 * --------------------------------------------------------------------------------------
 * Index Number - Telemetry Type                  - Telemetry Format
 * --------------------------------------------------------------------------------------
 *      0       - Unused                          - Unused
 *      1       - Unused                          - Unused
 *      2       - Unused                          - Unused
 *      3       - Unused                          - Unused
 *      4       - Unused                          - Unused
 *      5       - Unused                          - Unused
 *      6       - Unused                          - Unused
 *      7       - Unused                          - Unused
 *      8       - Unused                          - Unused
 *      9       - Unused                          - Unused
 *      10      - Unused                          - Unused
 *      11      - Unused                          - Unused
 *      12      - Unused                          - Unused
 *      13      - Unused                          - Unused
 *      14      - Unused                          - Unused
 *      15      - Unused                          - Unused
 *      16      - Unused                          - Unused
 *      17      - Unused                          - Unused 
 *      18      - Unused                          - Unused
 *      19      - Unused                          - Unused
 */
/////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////
//sensor variables
byte LiDAR_Counter = 0;
int altitude = 0;
float rollAngle = 0;
float pitchAngle = 0;
float convertedRoll = 0;
float convertedPitch = 0;
int convertedAltitude = 0;
float velocity = 0; /* m/s */
float latitude = 0;
float longitude = 0;
/////////////////////////////////////////////////////////////////////////////////////////




void setup()
{
  //initialize serial ports
  Serial.begin(2000000);  //PC debugging
  Serial2.begin(115200);  //3DR comms
  Serial3.begin(9600);    //GPS comms
  Serial4.begin(9600);    //XBee comms

  //turn on the Teensy PWR LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  //initialize radios
  commandsRadio.begin(Serial2);
  telemetryRadio.begin(Serial4);

  //initialize GPS
  myGPS.begin(Serial3);

  //initialize the IMU and wait for it to "boot up"
  if(!bno.begin())
  {
    //there was a problem detecting the BNO055 ... check your connections
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    //while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  //initialize the servo driver and set the frequency of all outputs to 50Hz
  pwm.begin();
  pwm.setPWMFreq(50);

  //initialize each individual servo to their respective center position
  pwm.setPWM(RUDDER_PIN, 0, (RUDDER_MAX+RUDDER_MIN)/2);
  pwm.setPWM(ELEVATOR_PIN, 0, (ELEVATOR_MAX+ELEVATOR_MIN)/2);
  pwm.setPWM(L_AILERON_PIN, 0, (AILERON_MAX+AILERON_MIN)/2);
  pwm.setPWM(R_AILERON_PIN, 0, (AILERON_MAX+AILERON_MIN)/2);
  
  //initialize the ESC
  throttle.attach(THROTTLE_PIN);

  //initialize the LiDAR for long distance readings
  myLidarLite.begin(0, true);
  myLidarLite.configure(0);
}




void loop()
{
  //receive data from the ground station if available
  getCommands();
  
  //read IMU data
  getEulerAngles();

  //read altimeter data
  readLiDAR();

  //read airspeed data
  readPitot();

  //find true altitude
  convertedAltitude = altitude * cos(convertedRoll) * cos(convertedPitch);

  //get position data if available
  if(myGPS.grabData_LatLong())
  {
    latitude = myGPS.GPS_data[0];
    longitude = myGPS.GPS_data[1];
  }
}




void getCommands()
{
  //get new data if available
  int report = commandsRadio.getData();

  //figure out if data was available - if so, determine if the transfer successful
  if(report == 1)
  {
    Serial.println("Success!");
    Serial.print("\tNew Data: ");
    for(byte i=0; i<DATA_LEN; i++)
    {
      if(i != 0)
      {
        Serial.print(", ");
      }
      
      Serial.print(commandsRadio.incomingArray[i]);
    }
    Serial.println();
  }
  else if(report == NO_DATA)
  {
    Serial.println("No Data Available");
  }
  else if(report == SERIAL_BUFF_ERROR)
  {
    Serial.println("ERROR - SERIAL_BUFF_ERROR");
  }
  else if(report == END_BYTE_ERROR)
  {
    Serial.println("ERROR - END_BYTE_ERROR");
  }
  else if(report == CHECKSUM_ERROR)
  {
    Serial.println("ERROR - CHECKSUM_ERROR");
  }
  else if(report == TIMEOUT_ERROR)
  {
    Serial.println("ERROR - TIMEOUT_ERROR");
  }
  else if(report == PAYLOAD_ERROR)
  {
    Serial.println("ERROR - PAYLOAD_ERROR");
  }
  else
  {
    Serial.print("ERROR - UNKNOWN ERROR OCCURRED: ");
    Serial.println(report);
  }
  
  return;
}




void getEulerAngles()
{
  //get a new sensor event
  sensors_event_t event;
  bno.getEvent(&event);

  //get pitch and roll angles in degrees
  rollAngle = event.orientation.y;
  pitchAngle = event.orientation.z;

  //convert Euler angles from degrees to radians - ONLY USED FOR LiDAR CORRECTION
  convertedRoll = (rollAngle) * (M_PI / 180);
  convertedPitch = (pitchAngle) * (M_PI / 180);
}




void readLiDAR()
{
  //get altitude readings - including periodic bias correction
  if(LiDAR_Counter >= 100)
  {
    altitude = myLidarLite.distance();
    LiDAR_Counter = 0;
  }
  else
  {
    altitude = myLidarLite.distance(false);
    LiDAR_Counter = LiDAR_Counter + 1;
  }
}




void readPitot()
{
  //velocity =  //(m/s)
  
  return;
}




void eulerLimit(byte angleType, int &processedData)
{
  if(angleType) ///////////////////////////pitch
  {
    if(pitchAngle < UNSAFEPITCHUP)
    {
      processedData = ELEVATOR_MAX;
    }
    else if(pitchAngle < MAXPITCHUP)
    {
      processedData = processedData + map(pitchAngle, MAXPITCHUP, UNSAFEPITCHUP, PITCH_CORRECTION_MIN, PITCH_CORRECTION_MAX);
      processedData = constrain(processedData, ELEVATOR_MIN, ELEVATOR_MAX);
    }
    else
    {
      //do nothing
    }
    
    if(pitchAngle > UNSAFEPITCHDOWN)
    {
      processedData = ELEVATOR_MIN;
    }
    else if(pitchAngle > MAXPITCHDOWN)
    {
      processedData = processedData - map(pitchAngle, MAXPITCHDOWN, UNSAFEPITCHDOWN, PITCH_CORRECTION_MIN, PITCH_CORRECTION_MAX);
      processedData = constrain(processedData, ELEVATOR_MIN, ELEVATOR_MAX);
    }
    else
    {
      //do nothing
    }
  }
  
  else ////////////////////////////////////roll
  {
    if(rollAngle < UNSAFEROLL_RIGHT)
    {
      processedData = AILERON_MAX;
    }
    else if(rollAngle < MAXROLL_RIGHT)
    {
      processedData = processedData + map(rollAngle, MAXROLL_RIGHT, UNSAFEROLL_RIGHT, ROLL_CORRECTION_MIN, ROLL_CORRECTION_MAX);
      processedData = constrain(processedData, AILERON_MIN, AILERON_MAX);
    }
    else
    {
      //do nothing
    }

    if(rollAngle < UNSAFEROLL_LEFT)
    {
      processedData = AILERON_MIN;
    }
    else if(rollAngle < MAXROLL_LEFT)
    {
      processedData = processedData - map(rollAngle, MAXROLL_LEFT, UNSAFEROLL_LEFT, ROLL_CORRECTION_MIN, ROLL_CORRECTION_MAX);
      processedData = constrain(processedData, AILERON_MIN, AILERON_MAX);
    }
    else
    {
      //do nothing
    }
  }
  
  return;
}













