/*  globals.h
      Define all user set values, global constants, global vars, included libraries and global objects

      Imogen Heard
      25/01/2025
*/


// Add included external libraries here (at the top of globals.h)
#include <Wire.h>
#include <SPI.h>
#include <stdlib.h>
#include <autoDelay.h>  //https://github.com/PanGalacticTech/autoDelay_library
#include <UstepperS32.h>
#include "TinyMPU6050.h"


// Program Attributes
#define EXPERIMENT_NAME         "dynamics-lab"
#define FIRMWARE_VERSION        "V0.0.0"
#define DEVELOPER               "Imogen-Heard"

// Hardware Definitions
#define HALL_SENSOR_PIN         A4
#define HALL_NORMALLY_HIGH      true  // define if normally high, triggered by low pulse (true) or normally low triggered by high pulse (false)

// User Options & program config
#define PRINT_RATE_Hz           40
#define PRINT_PERIODIC_UPDATES  true
#define ENCODE_RAW_ANGLE_OFFSET 0.0
#define STEPPER_HOLD_CURRENT    10  // percent
#define MAX_MOTOR_STEPS_S 800
#define MAX_MOTOR_ACC_STEPS_S_S 800


// Stall Detection Options
// this is definatly better done as a timer
// Stall count limit never reaches above 5 as position changes just enough to clear stall warning
#define STALL_COUNT_LIMIT 5  // Limit for typical number of stall events before triggering stall reset behaviour
// Variables to track total number of stalls and limit users to a defined number per time period
#define STALL_OPPORTUNITIES 3      // number of times stall reset behaivour can be triggered before motor is limited
#define STALL_COOL_DOWN_PERIOD 15  // cool down period to reset the number of triggered stalls
// When limit is reached, automatic stall guard is implemented by limiting how long the motor can stay in stall condition
#define PROTECT_STALL_COUNT_LIMIT 2  // Limit for number of stall events that trigger stall reset behaviour if protection mode has been activated





#define COMMAND_SIZE 64  // what command find better description


// Debugging Options
#define DEBUG_STATES            false
#define DEBUG_STATE_MACHINE     false
#define COMMAND_HINTS           false





// Add included internal libraries here
#include "jsonMessenger.h"

// Create objects
jsonMessenger jsonRX;  // create a json messenger object to handle commands received over Serial connection
autoDelay printDelay;  // Delay object for printing periodic JSON messages

UstepperS32 stepper;
MPU6050 mpu;


// Global Variables
uint32_t print_delay_mS = 1000 / PRINT_RATE_Hz;





// Add included internal header files here (at the bottom of globals.h)
#include "sdFunctions.h"
#include "matrixFunctions.h"
#include "pwmController.h"
#include "stateConfig.h"
