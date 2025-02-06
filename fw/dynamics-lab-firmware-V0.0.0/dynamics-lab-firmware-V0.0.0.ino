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
  //  std::cout << "\n{\"model\":\"" << EXPERIMENT_NAME << "\",\"version\":\"" << FIRMWARE_VERSION << "\",\"developed-by\":\"" << DEVELOPER << "\"}" << std::endl;
  Serial.print("\n{\"model\":\"");
  Serial.print(EXPERIMENT_NAME);
  Serial.print("\",\"version\":\"");
  Serial.print(FIRMWARE_VERSION);
  Serial.print("\",\"developed-by\":\"");
  Serial.print(DEVELOPER);
  Serial.println("\"}");
  jsonRX.jsonBegin();  // Start the json library to accept commands over serial connection
  mpu.Initialize();
  mpu.Calibrate();
  stepper_setup();
  servo.begin();
  servo.setInit(0);
  servo.setMin(-90);
  servo.setMax(20);
  servo.goMin();
  //servo_pos = false;
}


void loop() {


  jsonStateData nextState = jsonRX.jsonReadSerialLoop();  // If not using the queue function, then pass data from jsonMessenger object by passing values



  if (nextState.cmd_received) {  // If command is receive
    //delay(10);

    const char* cmd = jsonRX.getCMDkey(nextState.cmdState);  // I feel like the entire point of using ENUMs is being totally lost by doing this, but it is working
    //std::cout << std::endl;
    // std::cout << "{\"rx-cmd\":\"" << cmd << "\",\"datatype\":\"" << jsonRX.getDataType(nextState.data_type) << "\",\"data\":\"";
    Serial.print("{\"rx-cmd\":\"");
    Serial.print(cmd);
    Serial.print("\",\"datatype\":\"");
    Serial.print(jsonRX.getDataType(nextState.data_type));
    Serial.print("\",\"data\":\"");
    if (nextState.data_type == INTEGER) Serial.print(nextState.numeric);  //std::cout << nextState.numeric;
    if (nextState.data_type == FLOAT) Serial.print(nextState.floatData);  //std::cout << nextState.floatData;
    if (nextState.data_type == CSTRING) Serial.print(nextState.msg);      //std::cout << nextState.msg;
    if (nextState.data_type == EMPTY) Serial.print("n/a");                //std::cout << "n/a";
    // Is this now missing float clause?
    //std::cout << "\"}" << std::endl;
    Serial.println("\"}");

    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState.cmdState == STOP) {  // if fan speed change command received
      smState = STATE_STOP;
    } else if (nextState.cmdState == START) {
      smState = STATE_START;
    } else if (nextState.cmdState == SET_SPEED_HZ) {
      smState = STATE_SET_SPEED_HZ;
    } else if (nextState.cmdState == SET_SPEED_RPM) {
      smState = STATE_SET_SPEED_RPM;
    } else if (nextState.cmdState == HOME) {
      smState = STATE_HOME;
    } else if (nextState.cmdState == CALIBRATE) {
      smState = STATE_CALIBRATE;
    } else if (nextState.cmdState == FREEWHEEL) {
      smState = STATE_FREEWHEEL;
    } else if (nextState.cmdState == BRAKE) {
      smState = STATE_BRAKE;
    } else if (nextState.cmdState == GOTO) {
      smState = STATE_GOTO;
    } else if (nextState.cmdState == SAMPLERATE) {
      smState = STATE_SAMPLERATE;
    } else if (nextState.cmdState == STARTSTREAM) {
      smState = STATE_STARTSTREAM;
    } else if (nextState.cmdState == STOPSTREAM) {
      smState = STATE_STOPSTREAM;
    } else if (nextState.cmdState == PING) {
      smState = STATE_PING;
    } else if (nextState.cmdState == HELP) {
      smState = STATE_HELP;
    } else {
      std::cout << "{\"WARNING\":\"Unrecognised cmdState\"}" << std::endl;
    }
  }


  sm_Run(nextState);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command


  errors.clear_warning();  // clear JSON (move this to bottom of loop later)
  if (printDelay.millisDelay(10000)) {
    // std::cout << "alive" << std::endl;
    //Serial.println("Alive and Loop");
  }
}
