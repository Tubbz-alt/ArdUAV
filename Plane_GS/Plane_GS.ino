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

#define PITCH_ANALOG_PIN    3
#define ROLL_ANALOG_PIN     2
#define YAW_ANALOG_PIN      0
#define THROTTLE_ANALOG_PIN 1

#define PITCH_RATE_PIN  0
#define ROLL_RATE_PIN   1
#define YAW_RATE_PIN    2

#define AILERON_OFFSET  0
#define ELEVATOR_OFFSET 0
#define RUDDER_OFFSET   0
#define THROTTLE_OFFSET 0

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

#define THROTTLE_MIN_ADC  35880
#define AILERON_MIN_ADC   35300
#define ELEVATOR_MIN_ADC  36620
#define RUDDER_MIN_ADC    37330

#define THROTTLE_MAX_ADC  61220
#define AILERON_MAX_ADC   60180
#define ELEVATOR_MAX_ADC  62300
#define RUDDER_MAX_ADC    62170




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
  byte analog_pin;
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
  ROLL_COMMAND,           //command_index
  ROLL_ANALOG_PIN,        //analog_pin
  ROLL_RATE_PIN,          //rate_pin
  AILERON_OFFSET,         //_offset
  AILERON_MAX_HIGHRATES,  //high_rates_surface_max
  AILERON_MIN_HIGHRATES,  //high_rates_surface_min
  AILERON_MAX_LOWRATES,   //low_rates_surface_max
  AILERON_MIN_LOWRATES,   //low_rates_surface_min
  AILERON_MAX_ADC,        //ADC_max
  AILERON_MIN_ADC         //ADC_min
};

struct controlSurfaces elevator
{
  PITCH_COMMAND,          //command_index
  PITCH_ANALOG_PIN,       //analog_pin
  PITCH_RATE_PIN,         //rate_pin
  ELEVATOR_OFFSET,        //_offset
  ELEVATOR_MAX_HIGHRATES, //high_rates_surface_max
  ELEVATOR_MIN_HIGHRATES, //high_rates_surface_min
  ELEVATOR_MAX_LOWRATES,  //low_rates_surface_max
  ELEVATOR_MIN_LOWRATES,  //low_rates_surface_min
  ELEVATOR_MAX_ADC,       //ADC_max
  ELEVATOR_MIN_ADC        //ADC_min
};

struct controlSurfaces rudder
{
  YAW_COMMAND,            //command_index
  YAW_ANALOG_PIN,         //analog_pin
  YAW_RATE_PIN,           //rate_pin
  RUDDER_OFFSET,          //_offset
  RUDDER_MAX_HIGHRATES,   //high_rates_surface_max
  RUDDER_MIN_HIGHRATES,   //high_rates_surface_min
  RUDDER_MAX_LOWRATES,    //low_rates_surface_max
  RUDDER_MIN_LOWRATES,    //low_rates_surface_min
  RUDDER_MAX_ADC,         //ADC_max
  RUDDER_MIN_ADC          //ADC_max
};

struct controlSurfaces throttle
{
  THROTTLE_COMMAND,       //command_index
  THROTTLE_ANALOG_PIN,    //analog_pin
  0,                      //rate_pin
  THROTTLE_OFFSET,        //_offset
  THROTTLE_MAX,           //high_rates_surface_max
  THROTTLE_MIN,           //high_rates_surface_min
  0,                      //low_rates_surface_max
  0,                      //low_rates_surface_min
  THROTTLE_MAX_ADC,       //ADC_max
  THROTTLE_MIN_ADC        //ADC_max
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
const byte delayTime    = 25;

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

  /*Serial.print(commandsRadio.outgoingArray[0]); Serial.print(" ");
  Serial.print(commandsRadio.outgoingArray[1]); Serial.print(" ");
  Serial.print(commandsRadio.outgoingArray[2]); Serial.print(" ");
  Serial.print(commandsRadio.outgoingArray[3]); Serial.print(" ");
  Serial.println();*/

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
  uint16_t analogValue = analogRead(controlSurface.analog_pin);
  
  //Serial.print("ADC_min: "); Serial.println(controlSurface.ADC_min);
  //Serial.print("Analog: "); Serial.println(analogValue);
  //Serial.print("ADC_max: "); Serial.println(controlSurface.ADC_max);
  //Serial.print("high_rates_surface_min: "); Serial.println(controlSurface.high_rates_surface_min);
  
  //convert to a servo command format
  commandValue = map(analogValue,                             //value to be mapped
                      controlSurface.ADC_min,                 //input minimum expected value
                      controlSurface.ADC_max,                 //input maximum expected value
                      controlSurface.high_rates_surface_min,  //output minimum value
                      controlSurface.high_rates_surface_max   //output maximum value
                      ) + controlSurface._offset;             //add the offset (bias)
  
  //Serial.print("Command: "); Serial.println(commandValue);
  //Serial.print("high_rates_surface_max: "); Serial.println(controlSurface.high_rates_surface_max);
  //Serial.print("_offset: "); Serial.println(controlSurface._offset);
  
  commandValue = constrain(commandValue,                            //value to be constrained
                            controlSurface.high_rates_surface_min,  //minimum value
                            controlSurface.high_rates_surface_max   //maximum value
                            );

  //Serial.print("Command (clamped): "); Serial.println(commandValue);

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











