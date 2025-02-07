/*  globals.h
      Define all user set values, global constants, global vars, included libraries and global objects

      Imogen Heard
      25/01/2025
*/

#pragma once

// Add included external libraries here (at the top of globals.h)
#include <Wire.h>
#include <SPI.h>
//#include <iostream>
//#include <stdlib.h>
#include <autoDelay.h>  // https://github.com/PanGalacticTech/autoDelay_library
#include <UstepperS32.h>        // Arduino Library Manager (with additional boards manager ) https://raw.githubusercontent.com/uStepper/uStepperHardware/master/package_ustepper_index.json,https://raw.githubusercontent.com/uStepper/uStepperSTM32Hardware/master/package.json
#include "TinyMPU6050.h"  // Arduino Library Manager
#include <NewServo.h>          // Available @ https://github.com/GhassanYusuf/NewServo
#include "errorRep.h"
#include <ArduinoJson.h>           // installed version 6.21.5 [Arduino Library Manager]

// Program Attributes
#define EXPERIMENT_NAME "dynamics-lab"
#define FIRMWARE_VERSION "V0.0.0"
#define DEVELOPER "Imogen-Heard"

// Hardware Definitions
#define HALL_SENSOR_PIN A4
#define HALL_NORMALLY_HIGH true  // define if normally high, triggered by low pulse (true) or normally low triggered by high pulse (false)

// User Options & program config
#define PRINT_RATE_Hz 40
#define PRINT_PERIODIC_UPDATES true
#define ENCODE_RAW_ANGLE_OFFSET 0.0
#define STEPPER_HOLD_CURRENT 10  // percent
#define MAX_MOTOR_STEPS_S 800
#define MAX_MOTOR_ACC_STEPS_S_S 800
#define HOMING_TIMEOUT_S 10  // homing algorithm exits if home not found within this timeframe

#define PRINT_JSON true
#define PRETTY_PRINT_JSON false

#define MAX_RPM 1200
#define MAX_HZ 20

// Error System
#define WARNING_ACTIVE_PERIOD_mS 60000  // 1 min

// Stall Detection Options
// this is definatly better done as a timer
#define STALL_TIME_LIMIT_mS 2000


// Servo Options
#define SERVO_PPM_PIN 6
#define SERVO_ZERO_POS 0
#define SERVO_OPEN_POS 20



#define COMMAND_SIZE 64  // what command find better description


// Debugging Options
#define DEBUG_STATES true
#define DEBUG_STATE_MACHINE true
#define COMMAND_HINTS true





// Add included internal libraries here
#include "jsonMessenger.h"

// Create objects
jsonMessenger jsonRX;  // create a json messenger object to handle commands received over Serial connection
autoDelay printDelay;  // Delay object for printing periodic JSON messages // DEPRECIATED FOR NOW

autoDelay sampleDelay;
uint16_t sampleRate_Hz = 10;
uint32_t sampleDelay_mS = 1000/sampleRate_Hz;

UstepperS32 stepper;
MPU6050 mpu;
NewServo servo(SERVO_PPM_PIN);

errorRep errors;

#define JSON_BUFFER_SIZE 500
StaticJsonDocument<JSON_BUFFER_SIZE> jsonTX; 

// Global Variables
// Stepper Vars
float step_rpm = 0;
float step_hz = 0;

int16_t hall_low_point;
int16_t step_low_angle;

// Servo Vars
bool servo_pos = false;

// Sampling Vars
bool streaming_active = false;
uint16_t streaming_timer_mS = 0;

uint32_t print_delay_mS = 1000 / PRINT_RATE_Hz;





// Add included internal header files here (at the bottom of globals.h)

#include "stepperFunctions.h"
#include "stateConfig.h"
#include "jsonReporter.h"

