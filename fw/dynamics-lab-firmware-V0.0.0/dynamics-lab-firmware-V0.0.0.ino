/*  Dynamics Lab Firmware V0.0.0

Written By:
Imogen Heard
32/01/2025



*/

/* Version Control 



*/




#include "globals.h"







void setup() {
  Serial.begin(115200);
  std::cout << "\n{\"model\":\"" << EXPERIMENT_NAME  << "\",\"version\":\"" << FIRMWARE_VERSION << "\",\"developed-by\":\"" << DEVELOPER << "\"}" << std::endl;
  Serial.println();
  jsonRX.jsonBegin();  // Start the json library to accept commands over serial connection
  
}


void loop() {

  jsonStateData nextState = jsonRX.jsonReadSerialLoop();  // If not using the queue function, then pass data from jsonMessenger object by passing values



  if (nextState.cmd_received) {  // If command is receive

    const char* cmd = jsonRX.getCMDkey(nextState.cmdState);  // I feel like the entire point of using ENUMs is being totally lost by doing this, but it is working
    //std::cout << std::endl;
    std::cout << "{\"rx-cmd\":\"" << cmd << "\",\"datatype\":\"" << jsonRX.getDataType(nextState.data_type) << "\",\"data\":\"";
    if (nextState.data_type == INTEGER) std::cout << nextState.numeric;
    if (nextState.data_type == CSTRING) std::cout << nextState.msg;
    if (nextState.data_type == EMPTY) std::cout << "n/a";
    std::cout << "\"}" << std::endl;

    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState.cmdState >= FAN_A0 && nextState.cmdState <= FAN_B1) {  // if fan speed change command received
      smState = STATE_SET_OFFSET;
    } else if (nextState.cmdState == SPEED) {
      smState = STATE_SET_SPEED;
    } else if (nextState.cmdState == START) {
      smState = STATE_START;
    } else if (nextState.cmdState == STOP) {
      smState = STATE_STOP;
    } else if (nextState.cmdState == CAL) {
      smState = STATE_SET_CAL;
    } else if (nextState.cmdState == RUN) {
      smState = STATE_SET_RUN;
    } else if (nextState.cmdState == SAVE) {
      smState = STATE_SAVE;
    } else if (nextState.cmdState == DEL) {
      smState = STATE_DELETE;
    } else if (nextState.cmdState == PRINT) {
      smState = STATE_PRINT;
    } else if (nextState.cmdState == GET) {
      smState = STATE_GET;
    } else if (nextState.cmdState == HELP) {
      smState = STATE_HELP;
    } else {
      std::cout << "{\"WARNING\":\"Unrecognised cmdState\"}" << std::endl;
    }
  }


  sm_Run(nextState);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command


  if (printDelay.millisDelay(10000)) {
    // std::cout << "alive" << std::endl;
    // Serial.println("Alive and Loop");
  }
}
