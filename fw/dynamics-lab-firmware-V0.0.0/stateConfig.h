/*  stateConfig.h

This header should be used with the stateMachine library to define all the working states & execution logic for the stateMachine system

Imogen Heard
21/10/2024


*/


// Define the valid states for the state machine with an enum
typedef enum {
  STATE_INIT,
  STATE_WAIT,
  STATE_STOP,
  STATE_START,
  STATE_SET_SPEED,
  STATE_SET_OFFSET,
  STATE_SET_CAL,
  STATE_SET_RUN,
  STATE_SAVE,
  STATE_DELETE,
  STATE_PRINT,
  STATE_GET,
  STATE_HELP,
  NUM_STATES
} StateType;

//int NUM_STATES = 5;  // Not needed

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
  "STATE_SET_SPEED",
  "STATE_SET_OFFSET",
  "STATE_SET_CAL",
  "STATE_SET_RUN",
  "STATE_SAVE",
  "STATE_DELETE",
  "STATE_PRINT",
  "STATE_GET",
  "STATE_HELP"
};


// Define the state machine function prototypes
void sm_state_init(void);
void sm_state_wait(void);
void sm_state_stop(void);
void sm_state_start(void);
void sm_state_set_speed(jsonStateData stateData);
void sm_state_set_offset(jsonStateData stateData);
void sm_state_set_cal(void);
void sm_state_set_run(void);
void sm_state_save(void);
void sm_state_delete(jsonStateData stateData);
void sm_state_print(jsonStateData stateData);
void sm_state_get(jsonStateData stateData);
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
  Serial.println(F("{\"cmd\":\"Set offset for fan (at current speed)  -> \"}"));
  Serial.println(F("   {\"A0\": -255 to 255}"));
  Serial.println(F("   {\"C3\": -255 to 255}"));
  Serial.println(F("{\"cmd\":\"Set Global Fan Speed  -> \"}"));
  Serial.println(F("   {\"speed\": 0 to 255}"));
  Serial.println(F("{\"cmd\":\"Start & Stop Fans  -> \"}"));
  Serial.println(F("   {\"start\":0}"));
  Serial.println(F("   {\"stop\":0}"));
  Serial.println(F("{\"cmd\":\"Set Operational Modes ->\"}"));
  Serial.println(F("   {\"cal\":\"\"}"));
  Serial.println(F("   {\"run\":\"\"}"));
  Serial.println(F("{\"cmd\":\"View/Save/Delete Data ->\"}"));
  Serial.println(F("   {\"save\":\"\"}"));      // save current PWM_array to SD card
  Serial.println(F("   {\"del\":0 to 255}"));  // delete entry for {value} key from SD card
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
  set_global_pwm(0);
  std::cout << "{\"cmd\":\"stop\",\"speed\":\"" << 0 << "\"}" <<std::endl;
  smState = STATE_WAIT;
}



void sm_state_start(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: START"));
#endif
    lastState = smState;
  }
  if (calibration_mode) {
    std::cout << "{\"cmd\":\"start\",\"mode\":\"cal\",\"speed\":\"" << global_speed << "\"}" <<std::endl;
    update_pwm_with_array(PWM_array);
  } else {
    std::cout << "{\"cmd\":\"start\",\"mode\":\"run\",\"speed\":\"" << global_speed <<"\"}" <<std::endl;
    get_interpolated_matrix(global_speed);
    update_pwm_with_array(interpolated_array);
  }
  smState = STATE_WAIT;
}

// Set the Global Speed
void sm_state_set_speed(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_SPEED"));
#endif
    lastState = smState;
  }
  if (stateData.numeric >= 0 && stateData.numeric <= MAX_PWM_VAL) {
    global_speed = stateData.numeric;
    // Here it should make PWM array update with the existing matrix if it exists, or blank matrix
    // if it does not
    // this should ONLY affect operation during calibration mode
    if (calibration_mode) {
      extract_int_array_from_sd(global_speed, PWM_array);
      print_2d_named_array(PWM_array, "pwm_array");
    }
    std::cout << "{\"set\":\"speed\",\"to\":\"" << global_speed << "\"}" << std::endl;
  } else {
    std::cout << "{\"speed\":\"out of range\",\"still set to\":" << global_speed << "}" << std::endl;
  }
  smState = STATE_WAIT;
}



void sm_state_set_offset(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_OFFSET"));
#endif
    lastState = smState;
  }

  const char* cmd = jsonRX.getCMDkey(stateData.cmdState);  // look up the cstring for the command by using the cmdState enum I feel like the entire point of using ENUMs is being totally lost by doing this, but it is working
  uint16_t row = int(cmd[0] - 'A');                        // This turns the text passed from JSON into numbers
  uint16_t col = int(cmd[1] - '0');

  // std::cout << "row: " << row << " col: " << col << " data: " << stateData.numeric << std::endl;
  // Serial.println("Old Array: ");
  // print_2d_array(PWM_array);
  if (!calibration_mode) {
    Serial.println(F("{\"WARNING\":\"not in calibration mode, no offset applied\"}"));
  } else {
    if (stateData.numeric < MIN_OFFSET_VAL|| stateData.numeric > MAX_PWM_VAL) {
      Serial.println(F("{\"WARNING\":\"offset out of range\"}"));
    } else {
      if (update_2d_array(row, col, stateData.numeric)) {
        Serial.println(F("{\"success\":\"new offset applied\"}"));
      }
    }
    //Serial.println("New Array: ");
    print_2d_named_array(PWM_array, "new_array");
    //open_write_file(PWM_array);
  }
  smState = STATE_WAIT;
}



void sm_state_set_cal(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_CAL"));
#endif
    lastState = smState;
  }
  std::cout << "{\"mode\":\"cal\",\"speed\":\"" << global_speed << "\"}" <<std::endl;
  extract_int_array_from_sd(global_speed, PWM_array);  // get the current matrix for global speed
  print_2d_named_array(PWM_array, "pwm_array");
  calibration_mode = true;
  smState = STATE_WAIT;
}

void sm_state_set_run(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET_RUN"));
#endif
    lastState = smState;
  }
  calibration_mode = false;
  std::cout << "{\"mode\":\"run\",\"speed\":\"" << global_speed << "\"}" <<std::endl;
  smState = STATE_WAIT;
}

void sm_state_save(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SAVE"));
#endif
    lastState = smState;
  }
  if (!calibration_mode) {
    Serial.println(F("{\"WARNING\":\"Not in calibration mode, action blocked\"}"));
  } else {
    write_json_to_SD(global_speed, PWM_array);
    char speedName[8];
    sprintf(speedName, "%i", global_speed);
    print_2d_named_array(PWM_array, speedName);
  }
  smState = STATE_WAIT;
}


void sm_state_delete(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SAVE"));
#endif
    lastState = smState;
  }
  delete_json_entry(stateData.numeric);
  smState = STATE_WAIT;
}




void sm_state_print(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: PRINT"));
#endif
    lastState = smState;
  }
  if (strcmp(stateData.msg, "all") == 0) {
    print_all_memory();
  } else if (strcmp(stateData.msg, "pwm") == 0) {
    print_2d_named_array(PWM_array, "pwm_array");
  } else {
    Serial.println(F("{\"WARNING\":\"unable to parse cmd:{value}\"}"));
  }
  Serial.println();
  smState = STATE_WAIT;
}


void sm_state_get(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: GET"));
#endif
    lastState = smState;
  }
  print_record(stateData.numeric);
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
      case STATE_SET_SPEED:
        sm_state_set_speed(stateData);
        break;
      case STATE_SET_OFFSET:
        sm_state_set_offset(stateData);
        break;
      case STATE_SET_CAL:
        sm_state_set_cal();
        break;
      case STATE_SET_RUN:
        sm_state_set_run();
        break;
      case STATE_SAVE:
        sm_state_save();
        break;
      case STATE_DELETE:
        sm_state_delete(stateData);
        break;
      case STATE_PRINT:
        sm_state_print(stateData);
        break;
      case STATE_GET:
        sm_state_get(stateData);
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
