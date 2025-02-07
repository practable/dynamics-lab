/*  jsonConfig.h

This header should be used with the jsonMessenger library to define all the working states & commands that can be decoded by the jsonMessenger system

Please see: https://github.com/ImogenWren/jsonMessenger_library for latest version and usage instructions

Imogen Heard
21/10/2024


*/

//#pragma once

#ifndef jsonConfig_h
#define jsonConfig_h

#ifdef __AVR__
#include <ArduinoSTL.h>  // [Arduino Library Manager][Modified Version -> https://github.com/ImogenWren/ArduinoSTL]
#pragma "ArduinoSTL Libary Included"
#elif defined(STM32) || defined(ARDUINO_ARCH_STM32)
#pragma "STM32 Board -> Using Arrays Instead of std::stl"
#endif

#include <map>  // [std::map]


#define JSON_USE_QUEUE false        //At least one of these should be true
#define JSON_USE_SINGLE_FRAME true  // Single frame is always valid, but this can be used to disable features not wanted when using queue

#define JSON_RX_SIZE 32     // 32 Working on Arduino Nano
#define CMD_QUEUE_LENGTH 3  // 3 Working on Arduino Nano
#define JSON_MSG_LENGTH 8   // Length of msg array in json data structure

// Define an enum to define variable types, These will be linked to a state enum so when a keyword is received, we can look up what data type should be sent with it
typedef enum {  // enum to pass variable types between functions
  EMPTY,
  INTEGER,
  FLOAT,
  CSTRING,
  BOOL
} dataTypes;

const dataTypes dataTypes_array[5] = { EMPTY, INTEGER, FLOAT, CSTRING, BOOL };

// For human readability of enum above
static char typeNames[][8] = {
  "EMPTY",
  "INTEGER",
  "FLOAT",
  "CSTRING",
  "BOOL"
};


// Define list of typical commands for system for future reference
/*
{"A0": 100}
{"A1": 98}
{"all": "stop"}
{"all":0}

*/


// Declare a list of all possible key values as ENUM. These values will be passed out of the jsonMessenger Object and can be used to go to different states
// NOTE this list may not include all possible states, JUST the states that are triggered by receiving a command, including a null value at 0
// Why null value at 0? I cant remember but it seemed useful at the time
typedef enum {
  NONE,
  STOP,
  START,
  SET_SPEED_HZ,
  SET_SPEED_RPM,
  HOME,
  CALIBRATE,
  FREEWHEEL,
  BRAKE,
  GOTO,
  SAMPLERATE,
  STARTSTREAM,
  STOPSTREAM,
  PING,
  HELP,
  NUM_VALUES  // Add sentinal NUM_VALUES to count number of elements
} jsonStates;


// Now Link each jsonState ENUM with the datatype ENUM in a map structure.
//In this example most will be integers, but will include some cstrings to test
// As these types are just enums, we can try re-writing this using arrays to avoid using std::map, which is not fully implemented on some platforms
//const std::map<jsonStates, dataTypes> jsonStateMap = {
const uint16_t jsonStateMap[NUM_VALUES][2] = {
  { jsonStates::NONE, dataTypes::EMPTY },
  { jsonStates::STOP, dataTypes::EMPTY },
  { jsonStates::START, dataTypes::EMPTY },
  { jsonStates::SET_SPEED_HZ, dataTypes::FLOAT },
  { jsonStates::SET_SPEED_RPM, dataTypes::FLOAT },
  { jsonStates::HOME, dataTypes::EMPTY },
  { jsonStates::CALIBRATE, dataTypes::EMPTY },
  { jsonStates::FREEWHEEL, dataTypes::EMPTY },
  { jsonStates::BRAKE, dataTypes::EMPTY },
  { jsonStates::GOTO, dataTypes::INTEGER },
  { jsonStates::SAMPLERATE, dataTypes::INTEGER },
  { jsonStates::STARTSTREAM, dataTypes::EMPTY },
  { jsonStates::STOPSTREAM, dataTypes::EMPTY },
  { jsonStates::PING, dataTypes::EMPTY },
  { jsonStates::HELP, dataTypes::EMPTY }
};




// Then Declare a list of key commands that will be required to be parsed. This must match the order of the enums above
static char jsonCommandKeys[][7] = {
  "na",
  "stop",
  "start",
  "hz",
  "rpm",
  "home",
  "cal",
  "free",
  "brake",
  "goto",
  "sample",
  "stream",
  "endst",
  "ping",
  "help"
};
// NOTE, this can also be used to turn the enums above back into strings for human readability

// Also including generic keys, these are used for more verbose JSON commands like:
// {"set": "item", "to":"value"}
static char jsonGenerics[][5] = {
  "NULL",
  "set",
  "to",
  "get"
};



// Finally Declare a structure that will hold both the jsonStates enum, and any data that will need to be passed from jsonMessenger, into the states.
// We can make this fairly generic by including additional datatypes, or we can reduce the size of the memory used by removing the unneeded ones
struct jsonStateData {
  jsonStates cmdState;  // The command state enum to tell state machine what state to go to next
  dataTypes data_type;  // The type of data being passed along with structure (though state should know what data to expect anyway), this could be removed to save space
  int16_t numeric;      // empty generic data slots for each data type
  float floatData;
  char msg[JSON_MSG_LENGTH];
  bool cmd_received;  // Flag set true by jsonLoop when cmd is received
};





#endif