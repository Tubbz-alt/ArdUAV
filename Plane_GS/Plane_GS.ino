#include <AirComms.h>




#define PWR_LED_PIN 13

#define PITCH_COMMAND               0
#define ROLL_COMMAND                1
#define YAW_COMMAND                 2
#define THROTTLE_COMMAND            3
#define AUTOPILOT_TOGGLE_COMMAND    4
#define LIMITER_TOGGLE_COMMAND      4
#define LANDING_GEAR_TOGGLE_COMMAND 5
#define FLAPS_TOGGLE_COMMAND        5

#define PITCH_RATE_PIN  0
#define ROLL_RATE_PIN   1
#define YAW_RATE_PIN    2

#define AILERON_OFFSET  0
#define ELEVATOR_OFFSET 0
#define RUDDER_OFFSET   0

#define THROTTLE_MAX            170 //full throttle
#define AILERON_MAX_HIGHRATES   320 //roll left
#define ELEVATOR_MAX_HIGHRATES  310 //nose up
#define RUDDER_MAX_HIGHRATES    340 //nose left

#define THROTTLE_MIN            50  //no throttle
#define AILERON_MIN_HIGHRATES   230 //roll right
#define ELEVATOR_MIN_HIGHRATES  230 //nose down
#define RUDDER_MIN_HIGHRATES    220 //nose right

#define AILERON_MAX_LOWRATES  325
#define ELEVATOR_MAX_LOWRATES 350
#define RUDDER_MAX_LOWRATES   292

#define AILERON_MIN_LOWRATES  289
#define ELEVATOR_MIN_LOWRATES 264
#define RUDDER_MIN_LOWRATES   264

#define THROTTLE_MIN_ADC  36960
#define AILERON_MIN_ADC   36300
#define ELEVATOR_MIN_ADC  37550
#define RUDDER_MIN_ADC    38380

#define THROTTLE_MAX_ADC  62800
#define AILERON_MAX_ADC   61700
#define ELEVATOR_MAX_ADC  63710
#define RUDDER_MAX_ADC    63800




//create instances of the AirComms class
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
//structs to handle control surface commands
struct controlSurfaces
{
  byte command_index;
  byte rate_pin;
  uint16_t _offset;
  uint16_t high_rates_surface_max;
  uint16_t high_rates_surface_min;
  uint16_t low_rates_surface_max;
  uint16_t low_rates_surface_min;
  uint16_t ADC_max;
  uint16_t ADC_min;
};

struct controlSurfaces ailerons
{
  ROLL_COMMAND, ROLL_RATE_PIN, AILERON_MAX_HIGHRATES, AILERON_MIN_HIGHRATES, 
  AILERON_MAX_LOWRATES, AILERON_MIN_LOWRATES, AILERON_MAX_ADC, AILERON_MIN_ADC
};

struct controlSurfaces elevator
{
  PITCH_COMMAND, PITCH_RATE_PIN, ELEVATOR_MAX_HIGHRATES, ELEVATOR_MIN_HIGHRATES, 
  ELEVATOR_MAX_LOWRATES, ELEVATOR_MIN_LOWRATES, ELEVATOR_MAX_ADC, ELEVATOR_MIN_ADC
};

struct controlSurfaces rudder
{
  YAW_COMMAND, YAW_RATE_PIN, AILERON_MAX_HIGHRATES, AILERON_MIN_HIGHRATES, 
  AILERON_MAX_LOWRATES, AILERON_MIN_LOWRATES, AILERON_MAX_ADC, AILERON_MIN_ADC
};

struct controlSurfaces throttle
{
  THROTTLE_COMMAND, PWR_LED_PIN, THROTTLE_MAX, THROTTLE_MIN, 0, 0, THROTTLE_MAX_ADC, 
  THROTTLE_MIN_ADC
};
/////////////////////////////////////////////////////////////////////////////////////////




//array to identify which datafields need to be sent to the plane
bool commandsArray[DATA_LEN] = {false};

//analog pin assignments
const byte yawPin       = 0;
const byte throttlePin  = 1;
const byte rollPin      = 2;
const byte pitchPin     = 3;

//loop total delay in ms
const byte delayTime    = 2;

//variables for control surface values
unsigned int pitchValue     = 0;
unsigned int rollValue      = 0;
unsigned int yawValue       = 0;
unsigned int throttleValue  = 0;




void setup()
{
  //turn on the Teensy PWR LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //setup rate switch inputs
  pinMode(PITCH_RATE_PIN, INPUT_PULLUP);  //pitch rates
  pinMode(ROLL_RATE_PIN, INPUT_PULLUP);   //roll rates
  pinMode(YAW_RATE_PIN, INPUT_PULLUP);    //rudder rates

  //take analog readings at a resolution of 16-bits
  analogReadResolution(16);

  //initialize serial ports
  Serial.begin(2000000);
  Serial4.begin(115200);  //3DR radio
  Serial5.begin(9600);    //XBee radio

  //initialize radios
  commandsRadio.begin(Serial4);
  telemetryRadio.begin(Serial5);
}




void loop()
{
  //DAQ from joystick/switches
  updateCommands();

  //send data to plane IFC
  sendData();

  //wait for IFC to "catch up"
  delay(delayTime);
}




void updateCommands()
{
  //read and map joystick data
  //mapping normalized to max and min joystick values

  updateCommand(ailerons);
  updateCommand(elevator);
  updateCommand(rudder);
  updateCommand(throttle);

  return;
}




void updateCommand(controlSurfaces controlSurface)
{
  //value to be sent to plane
  uint16_t commandValue = 0;
  
  //read the analog voltage of the potentiometer
  uint16_t analogValue = analogRead(controlSurface.rate_pin);

  //convert to a servo command format
  commandValue = map(analogValue, controlSurface.ADC_min, controlSurface.ADC_max, controlSurface.high_rates_surface_max, controlSurface.high_rates_surface_min) + controlSurface._offset;
  commandValue = constrain(analogValue, controlSurface.high_rates_surface_min, controlSurface.high_rates_surface_max);

  //set the respective commandArray index to true so that the servo data will be sent to
  //the plane
  commandsArray[controlSurface.command_index] = true;

  //update the AirComms out buffer
  commandsRadio.outgoingArray[controlSurface.command_index] = commandValue;

  return;
}




void sendData()
{
  //send commands to the plane
  commandsRadio.sendData(commandsArray);

  //reset boolean array
  resetCommandsArray();
  
  return;
}




void resetCommandsArray()
{
  for(byte i=0; i<DATA_LEN; i++)
  {
    commandsArray[i] = false;
  }
  
  return;
}











