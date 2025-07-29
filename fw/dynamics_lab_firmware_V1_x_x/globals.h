/*  globals.h
      Define all user set values, global constants, global vars, included libraries and global objects

      Imogen Heard
      25/01/2025
*/

#pragma once

// Add included external libraries here (at the top of globals.h)
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
//#include <iostream>
//#include <stdlib.h>
#include <autoDelay.h>    // https://github.com/PanGalacticTech/autoDelay_library
#include <UstepperS32.h>  // Arduino Library Manager (with additional boards manager ) https://raw.githubusercontent.com/uStepper/uStepperHardware/master/package_ustepper_index.json,https://raw.githubusercontent.com/uStepper/uStepperSTM32Hardware/master/package.json
//#include "TinyMPU6050.h"  // Arduino Library Manager
//#include <NewServo.h>     // Available @ https://github.com/GhassanYusuf/NewServo // NOTE ERRORS POSSIBLY CAUSED BY THIS LIBRARY
#include <Servo.h>  // [Arduino Library Manager]
#include "errorRep.h"
#include <ArduinoJson.h>  // installed version 6.21.5 [Arduino Library Manager]
#include <ledObject.h>
#include "secretObject.h"


//#include <FlashStorage_STM32.h>

// Program Attributes
#define EXPERIMENT_NAME "dynamics-lab"
#define FIRMWARE_VERSION "V1.1.0"
#define DEVELOPER "Imogen-Heard"

// Hardware Definitions
#define HALL_SENSOR_PIN A4
#define HALL_NORMALLY_HIGH true  // define if normally high, triggered by low pulse (true) or normally low triggered by high pulse (false)
#define STEPPER_STEPS 200
#define LED_BEACON 4

// User Options & program config
#define INIT_SAMPLE_RATE_Hz 75
#define INIT_PRINT_RATE_Hz 15
const int RUNNING_MODE_TIMEOUT_S = 300;     // Times out running mode/movement after time delay from recieving last command
const int FREEWHEEL_BRAKE_TIMEOUT_S = 600;  // times out the brake mode and prevents motor heating when not in use



#define STREAMING_DEFAULT_ACTIVE false
#define ENCODE_RAW_ANGLE_OFFSET 0.0
#define STEPPER_HOLD_CURRENT 10  // percent
#define MAX_MOTOR_STEPS_S 800
#define MAX_MOTOR_ACC_STEPS_S_S 300    // normal running mode accelleration
#define MAX_MOTOR_ACC_SETUP 1000 // max accelleration during setup and calibrations
#define HOMING_TIMEOUT_S 10  // homing algorithm exits if home not found within this timeframe

#define ENCODER_HOME_OFFSET 5830
#define PRINT_HOMING_RESULT false

#define PRINT_JSON true
#define PRETTY_PRINT_JSON false

#define MAX_RPM 1200
#define MAX_HZ 20

// Error System
#define WARNING_ACTIVE_PERIOD_mS 60000  // 1 min

// Stall Detection Options
// this is definatly better done as a timer
#define STALL_TIME_LIMIT_mS 2000  // DONT THINK THIS IS USED ANYMORE


// Servo Options
#define SERVO_PPM_PIN 6
#define SERVO_ZERO_uS 800
#define SERVO_OPEN_uS 2200



#define COMMAND_SIZE 64  // what command find better description


// Debugging Options
#define DEBUG_STATES true         // not JSON safe
#define DEBUG_STATE_MACHINE true  // JSON safe
#define COMMAND_HINTS false

// Physics Constants
#define G_CONST 9.80665


// Utility
#define EEPROM_ENCODE_OFFSET_ADDRESS 0xFF  // The first 15 values in EEprom are used by uStepper lib. 0xFF is arbitraty address away from first 15 addresses




// Add included internal libraries here
#include "jsonMessenger.h"

// Create objects
jsonMessenger jsonRX;  // create a jsonMessenger object to handle commands received over Serial connection
UstepperS32 stepper;   // uStepper32 control for stepper motor
//MPU6050 mpu;   /// replaced with Adafruit library
Adafruit_MPU6050 mpu;
Servo servo;
errorRep errors;
ledObject beacon(LED_BEACON);
secretObject memory;

secretObject::calStruc cal;


// why is this defined in globals?!?
// moving it to local
#define JSON_TX_BUFFER_SIZE 10000
//StaticJsonDocument<JSON_TX_BUFFER_SIZE> jsonTX;



// Global Variables
// Global Timers
uint32_t last_command_rx_mS = 0;

// Stepper State
typedef enum {  // enum to pass variable types between functions
  STOPPED,
  RUNNING,
  STALLED,
  FREE,
  MOMENTARY  // momentary state to timeout after 10 seconds, ignores stall warning
} stepState;

stepState motorState = STOPPED;
// Stepper Vars
float step_rpm = 0;
float step_hz = 0;

// Encoder Variables
int16_t hall_low_point;
int16_t step_low_angle;
int16_t persistant_encoder_offset;
uint16_t last_encoder_value = 0;
int16_t stalls_detected = 0;
const int16_t stall_limit = 30;  // number of times stall can be triggered before registering as a full stall set to high enough number not to trigger on start
//uint32_t stall_trigger_time_mS;
uint32_t last_stall_time_mS;
#define STALL_TIME_OUT_mS 2000
#define STALL_HIT_DECAY_mS 200

float goto_target = 0;                      // goto state sets global var then uses this while remaining in goto state until target position has been reached
const float GOTO_TARGET_HYSTERESIS = 0.15;  // <- Asperational  // working & reliable -> 0.41
bool goto_triggered = false;                // DONT THINK THIS IS NEEDED
float target_lower;
float target_higher;
int16_t origional_target;  // just used to check result later in cycle

//EEprom Variables
const int WRITTEN_SIGNATURE = 0x98C7AB1E;  // Arbitary signature to check for existing encoder offset value in persistant memory (practable)

// Servo Vars
bool servo_pos = false;
uint32_t servo_attach_time_mS = 0;
#define SERVO_TIMEOUT_mS 3000

// Sampling Vars
bool streaming_active = STREAMING_DEFAULT_ACTIVE;
bool snapshop_active = false;
uint16_t snapshot_timer_mS = 25000;
uint32_t snapshot_starttime_mS;

#define SAMPLE_DELAY_OFFSET 0  // offset in mS

autoDelay sampleDelay;
uint16_t sampleRate_Hz = INIT_SAMPLE_RATE_Hz;
//uint32_t sampleDelay_mS = uint32_t(1000 / sampleRate_Hz) - 4;  // added -5 to make the delay just a little shorter, and ensure we get all samples in before they are sent.
uint32_t sampleDelay_mS = uint32_t(1E3 / sampleRate_Hz) - SAMPLE_DELAY_OFFSET;  //
// The function will stop sampling once buffer it full, so this should make more consistant outputs
// #TODO MAKE SURE STATE FUNCTION IS UPDATED TO MATCH

autoDelay printDelay;
uint16_t print_rate_Hz = INIT_PRINT_RATE_Hz;
uint32_t print_delay_mS = uint32_t(1000 / print_rate_Hz);

uint8_t num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle

// Maximum sample rate of 200Hz, minimum print rate of 1Hz, so max number of data to be collected is 200 samples x 8 datapoints, 4 bytes per float = 5600 bytes. Very doable data levels on STM32, HOWEVER;
// remember all this data will need to be copied to JSON so lets double that 11200! -> still very doable on STM32!

// but in the future, this must be restricted
// on smaller microcontrollers. Suggest limit of 4, so print rate cant even be less than 4 times sample rate. This can be defined by DATA_ARRAY_SIZE, and will (later) be used to resitrict sample rates and print rates to
// stay within this value, each time one is updated it must be checked for conformity to this against the other.

#define DATA_ARRAY_SIZE 200

uint32_t timestamp_array[DATA_ARRAY_SIZE];
float encode_array[DATA_ARRAY_SIZE];
float accX_array[DATA_ARRAY_SIZE];
float accY_array[DATA_ARRAY_SIZE];
float accZ_array[DATA_ARRAY_SIZE];
float gyroX_array[DATA_ARRAY_SIZE];
float gyroY_array[DATA_ARRAY_SIZE];
float gyroZ_array[DATA_ARRAY_SIZE];

int16_t samples_written = 0;

struct accOffsets {
  float X;
  float Y;
  float Z;
} acc_offset = { 0.0, 0.0, 0.0 };  //{ 0.101, -0.02, -0.147 };

struct gyroOffsets {
  float X;
  float Y;
  float Z;
} gyro_offset = { 0, 0, 0 };



//uint32_t print_delay_mS = 1000 / PRINT_RATE_Hz;

// Utility functions
void switchVariables(float &varA, float &varB) {
  float b;
  b = varA;
  varA = varB;
  varB = b;
}



// Add included internal header files here (at the bottom of globals.h)

#include "persistantMemory.h"
#include "stepperFunctions.h"
#include "mpuFunctions.h"
#include "demo.h"
#include "stateConfig.h"
#include "jsonReporter.h"
#include "trackRAM_stm.h"
