/*  stateConfig.h

This header should be used with the stateMachine library to define all the working states & execution logic for the stateMachine system

Imogen Heard
21/10/2024


*/
#pragma once

#include <ArduinoJson.h>


// Define the valid states for the state machine with an enum
typedef enum {
  STATE_INIT,
  STATE_WAIT,
  STATE_STOP,
  STATE_START,
  STATE_SET_SPEED_HZ,
  STATE_SET_SPEED_RPM,
  STATE_HOME,
  STATE_CALIBRATE,
  STATE_FREEWHEEL,
  STATE_BRAKE,
  STATE_GOTO,
  STATE_SAMPLERATE,
  STATE_STARTSTREAM,
  STATE_STOPSTREAM,
  STATE_PING,
  STATE_HELP,
  NUM_STATES  // Guard value lets us get the total number of states without manually counting
} StateType;


// Then we define two variables to hold the current state enum, and the previous state enum. Holding both these values allows us to compare them as we enter a state,
// to see if it is the first time we have entered the state.

StateType smState = STATE_INIT;
StateType lastState;




// Then define a State Names Array, this will allow us to print the enum above in human readable format
char stateNames[][17] = {
  "STATE_INIT",
  "STATE_WAIT",
  "STATE_STOP",
  "STATE_START",
  "STATE_SET_SPEED_HZ",
  "STATE_SET_SPEED_RPM",
  "STATE_HOME",
  "STATE_CALIBRATE",
  "STATE_FREEWHEEL",
  "STATE_BRAKE",
  "STATE_GOTO",
  "STATE_SAMPLERATE",
  "STATE_STARTSTREAM",
  "STATE_STOPSTREAM",
  "STATE_PING",
  "STATE_HELP"
};


// Define the state machine function prototypes
void sm_state_init(void);
void sm_state_wait(void);
void sm_state_stop(void);
void sm_state_start(void);
void sm_state_set_speed_hz(jsonStateData stateData);
void sm_state_set_speed_rpm(jsonStateData stateData);
void sm_state_home(void);
void sm_state_calibrate(void);
void sm_state_freewheel(void);
void sm_state_brake(void);
void sm_state_goto(jsonStateData stateData);
void sm_state_samplerate(jsonStateData stateData);
void sm_state_start_stream(jsonStateData stateData);
void sm_state_stop_stream(void);
void sm_state_ping(void);
void sm_state_help(void);





// Define the state machine functions
//  A typical state function template is shown below
/*
void sm_state_template(void) {
  if (lastState != smState) {  // Check to see if first time state has been called in sequence
    // Do anything that needs to happen the First time this state is called
    lastState = smState;  // Set the last state to the current state
  }
  // Do everything that repeats as long as this state is active

  // Add any logic to navigate to other states
  smState = STATE_WAIT;
}
*/




void sm_state_init() {
  if (lastState != smState) {
    Serial.println("state: init");
  }
  smState = STATE_WAIT;
}


void print_cmds() {
  Serial.println(F("   {\"A0\": -255 to 255}"));
  Serial.println(F("   {\"C3\": -255 to 255}"));
  Serial.println(F("   {\"speed\": 0 to 255}"));
  Serial.println(F("   {\"start\":0}"));
  Serial.println(F("   {\"stop\":0}"));
  Serial.println(F("   {\"cal\":\"\"}"));
  Serial.println(F("   {\"run\":\"\"}"));
  Serial.println(F("   {\"save\":\"\"}"));      // save current PWM_array to SD card
  Serial.println(F("   {\"del\":0 to 255}"));   // delete entry for {value} key from SD card
  Serial.println(F("   {\"print\":\"all\"}"));  // Print all JSON data on SD card
  Serial.println(F("   {\"print\":\"pwm\"}"));  // Print current PWM_array
  Serial.println(F("   {\"get\": 0 to 255}"));  // get record for {value} key from SD card
  Serial.println(F("   {\"help\":\"\"}"));      // Print commands list
}

// State Wait is the default state for this program
void sm_state_wait() {
  if (lastState != smState) {
    // If first iteration print state machine status
#if DEBUG_STATES == true
    Serial.println(F("state: WAIT"));
#endif

#if COMMAND_HINTS == true  // print suggested commands
    Serial.println(F("\nEnter cmd in format:"));
    print_cmds();
#endif
    lastState = smState;
  }
}




void sm_state_stop(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOP"));
#endif
    lastState = smState;
  }
  // stepper.stop(HARD);
  //  //stepper.setRPM(0);
  step_rpm = 0;
  step_hz = 0;
  smState = STATE_WAIT;
}



void sm_state_start(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: START"));
#endif
    lastState = smState;
  }
  //  //stepper.setRPM(step_rpm * -1);  // invert movement so clockwise is positive
  smState = STATE_WAIT;
}





// Set the Global Speed
void sm_state_set_speed_hz(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_SPEED_HZ"));
#endif
    lastState = smState;
  }
  if (stateData.floatData < (-1 * MAX_HZ) || stateData.floatData > MAX_HZ) {  // if value is out of range, reject
    set_error(false, -10, "Requested Hz value out of bounds", WARNING, "set_speed_hz");
    print_json_status(true);
  } else {
    step_hz = stateData.floatData;
 //   step_rpm = get_RPM_from_Hz(step_hz);
    clear_error(-10);
  }
  smState = STATE_WAIT;
}



void sm_state_set_speed_rpm(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_SPEED_RPM"));
#endif
    lastState = smState;
  }
  if (stateData.floatData < (-1 * MAX_RPM) || stateData.floatData > MAX_RPM) {  // if value is out of range, reject
    set_error(false, -10, "Requested RPM value out of bounds", WARNING, "set_speed_rpm");
    print_json_status(true);
  } else {
    step_rpm = stateData.floatData;
 //   step_hz = get_Hz_from_RPM(step_rpm);
    clear_error(-10);
  }
  smState = STATE_WAIT;
}






// Send motor to home position
void sm_state_home(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: HOME"));
#endif
    lastState = smState;
  }
//  step_move_home();
  smState = STATE_WAIT;
}

void sm_state_calibrate(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: CALIBRATET"));
#endif
    lastState = smState;
  }
//  step_find_home();
 // step_move_home();
  smState = STATE_WAIT;
}








void sm_state_freewheel(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: FREEWHEEL"));
#endif
    lastState = smState;
  }
 // stepper.setBrakeMode(FREEWHEELBRAKE);
  smState = STATE_WAIT;
}



void sm_state_brake(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: BRAKE"));
#endif
    lastState = smState;
  }
//  stepper.setBrakeMode(COOLBRAKE);
  smState = STATE_WAIT;
}








void sm_state_goto(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SAVE"));
#endif
    lastState = smState;
  }
//  stepper.moveToAngle(jsonStateData.floatData);
  smState = STATE_WAIT;
}





void sm_state_ping(void){
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SAVE"));
#endif
    lastState = smState;
  }
  if (servo_pos) {
    servo.goMin();
    servo_pos = false;
  } else {
    servo.goMax();
    servo_pos = true;
  }
  smState = STATE_WAIT;
}
//TODO: Go to stream data snapshot mode?




void sm_state_samplerate(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SAMPLERATE"));
#endif
    lastState = smState;
  }

  smState = STATE_WAIT;
}

void sm_state_start_stream(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STARTSTREAM"));
#endif
    lastState = smState;
  }
  streaming_active = true;
//  streaming_timer_mS = jsonStateData.numeric;
  smState = STATE_WAIT;
}



void sm_state_stop_stream(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOPSTREAM"));
#endif
    lastState = smState;
  }
  streaming_active = false;
  streaming_timer_mS = 0;
  smState = STATE_WAIT;
}


void sm_state_help(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    lastState = smState;
  }
  print_cmds();
  smState = STATE_WAIT;
}




// Finally define the state machine function
// Automatically generate the switch case from the list of ENUM states and list of functions! -> https://github.com/ImogenWren/switch-case-generator
void sm_Run(jsonStateData stateData) {
  if (smState < NUM_STATES) {
#if DEBUG_STATE_MACHINE == true
    if (lastState != smState) {
      Serial.print(F("{\"statemachine\":\""));
      Serial.print(stateNames[smState]);
      Serial.println(F("\"}"));
    }
#endif
    switch (smState) {
      case STATE_INIT:
        sm_state_init();
        break;
      case STATE_WAIT:
        sm_state_wait();
        break;
      case STATE_STOP:
        sm_state_stop();
        break;
      case STATE_START:
        sm_state_start();
        break;
      case STATE_SET_SPEED_HZ:
        sm_state_set_speed_hz(stateData);
        break;
      case STATE_SET_SPEED_RPM:
        sm_state_set_speed_rpm(stateData);
        break;
      case STATE_HOME:
        sm_state_home();
        break;
      case STATE_CALIBRATE:
        sm_state_calibrate();
        break;
      case STATE_FREEWHEEL:
        sm_state_freewheel();
        break;
      case STATE_BRAKE:
        sm_state_brake();
        break;
      case STATE_GOTO:
        sm_state_goto(stateData);
        break;
      case STATE_SAMPLERATE:
        sm_state_samplerate(stateData);
        break;
      case STATE_STARTSTREAM:
        sm_state_start_stream(stateData);
        break;
      case STATE_STOPSTREAM:
        sm_state_stop_stream();
        break;
      case STATE_PING:
        sm_state_ping();
        break;
      case STATE_HELP:
        sm_state_help();
        break;
      default:
        sm_state_stop();
        Serial.println(F("sm: Exception -> default"));
        break;
    }
  } else {
    sm_state_stop();
    Serial.println(F("sm: Exception -> NUM_STATES exceeded"));
  }
}
