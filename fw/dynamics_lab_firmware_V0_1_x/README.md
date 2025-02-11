# dynamics-lab-firmware
_Control a stepper motor and servo via JSON formatted command set. Gather data from position encoder and accellerometer mpu_

## Commands list
_List of basic commands in JSON format. This list is printed out on entry to STATE_WAIT and by entering STATE_HELP

_Verbose Command Structure_
```
   {"set":"start"}                   -> Start/Update Motor Speed
   {"set":"stop"}                    -> Stop Motor              
   {"set":"hz","to": -20 to 20}      -> Set Motor Speed in Hz   
   {"set":"rpm","to" -200 to 200}    -> Set Motor Speed in RPM  
   {"set":"home"}                    -> Move Motor to home pos (test) 
   {"set":"cal"}                     -> Run Calibration to home motor 
   {"set":"free"}                    -> Set freewheel brake mode (test)
   {"set":"brake"}                   -> Set coolbrake brake mode (test)
   {"set":"goto","to": -360 to 360}  -> Goto Angle (test)              
   {"set":"sample","to": 1 to 40}    -> Set Samplerate in Hz         
   {"set":"stream"}                  -> Start Data Streaming    
   {"set":"endst"}                   -> End Data Streaming      
   {"set":"snap"}                    -> Take Data Snapshot       
   {"set":"time","to": 1 - 250000 }  -> Set Time for Data Snapshot (mS)  
   {"set":"ping"}                    -> Ping Servo               
   {"set":"help"}                    -> Print Commands to Serial Monitor 


```


_Current Functional Commands_
```
   {"start":}          -> Start/Update Motor Speed
   {"stop":}           -> Stop Motor              
   {"hz": -20 to 20}    -> Set Motor Speed in Hz   
   {"rpm": -200 to 200} -> Set Motor Speed in RPM  
   {"home":}        -> Move Motor to home pos (test) 
   {"cal":}         -> Run Calibration to home motor 
   {"free":}        -> Set freewheel brake mode (test)
   {"brake":}       -> Set coolbrake brake mode (test)
   {"goto": -360 to 360}-> Goto Angle (test)              
   {"sample": 1 to 40}  -> Set Samplerate in Hz         
   {"stream":}      -> Start Data Streaming    
   {"endst":}       -> End Data Streaming      
   {"snap":}        -> Take Data Snapshot       
   {"time": 1 - 250000 }-> Set Time for Data Snapshot (mS)  
   {"ping":}        -> Ping Servo               
   {"help":}        -> Print Commands to Serial Monitor 

```





## jsonMessenger_library
_This library has been developed to simplify the development of embedded state machines with a JSON formatted command structure for sending commands and receiving data over a Serial interface_

It works by associating different keywords used to define commands sent by a user or user interface, with a list of known states, that could represent all, or some of the various states of a state machine, and 
a list of the expected variable types to be sent along with the defined keywords.



 ## Library Use
_The library is made up of several distinct parts_
1. `stateConfig.h` -> User Editable File: Defines the state machine running constantly on the embedded system. Every potential state should be described in this user editable file
2. `jsonConfig.h`  -> User Editable File: Defines the user commands, datatypes and all states that can be triggered via the jsonMessenger system, but does not need to define any states that are not linked with a Serial command
3. `jsonMessenger.h` -> jsonMessenger Library Header File, make sure to include this in `globals.h`. This file is not intended to be user editable, and can be placed in `libraries` along with `jsonMessenger.cpp`
4. `void loop() function` -> This function must call  `jsonMessenger::jsonReadSerialLoop();` and `sm_Run` and includes a small `if/if else` structure to tie jsonMessenger to the function of the state machine

Please Find detailed instruction on use and modification of the user editable files, and the code required inside the loop() function below.

### Modifying `stateConfig.h`
_Defines the state machine running constantly on the embedded system. Every potential state should be described in this user editable file_ Note: This template can be used on its own without jsonMessenger if
the functions of the jsonMessenger libary are not required.


#### 1. Define all the valid states for the state machine with an enum
//    - This should include all states triggered by user input, but may also contain states that are only accessable programatically
```
typedef enum {
  STATE_INIT,
  STATE_WAIT,
  STATE_STOP,
  STATE_START,
  STATE_SET_SPEED_HZ,
  STATE_SET_SPEED_RPM,
  STATE_PING,
  STATE_HELP,
  NUM_STATES  // Sentinal value lets us get the total number of states without manually counting. Do not forget this value, it is important for correct function
} StateType;
```

#### 2. Define variables to hold the current, and previous state enum
Holding both these values allows us to compare them as we enter a state, to see if it is the first time we have entered the state.

```
StateType smState = STATE_INIT;  // This variable also defines the initial state
StateType lastState;
```

#### 3. Define a State Names Array
This will allow us to print the enum above in human readable format
```
char stateNames[][20] = {
  "STATE_INIT",
  "STATE_WAIT",
  "STATE_STOP",
  "STATE_START",
  "STATE_SET_SPEED_HZ",
  "STATE_SET_SPEED_RPM",
  "STATE_PING",
  "STATE_HELP"
};
```

#### 4. Define the state machine function prototypes
Any function that will be passed data from a user input will be passed the same jsonStateData structure as an argument
```
void sm_state_init(void);
void sm_state_wait(void);
void sm_state_stop(void);
void sm_state_start(void);
void sm_state_set_speed_hz(jsonStateData stateData);
void sm_state_set_speed_rpm(jsonStateData stateData);
void sm_state_ping(void);
void sm_state_help(void);
```




#### 5. Define all the state machine functions
 _A typical state function template is shown below_

```
void sm_state_template(jsonStateData stateData) {

  if (lastState != smState) {  // Check to see if first time state has been called in sequence
    // 5a. Do anything that needs to happen the First time this state is called
    lastState = smState;  // Set the last state to the current state
  }

  // 5b. Do everything that repeats as long as this state is active

  // 5c. Extract data from jsonStateData (if required)
          - remember to validate data if coming from user input!

  if (stateData.numeric < 1 || stateData.numeric > 2000){
    // Error message to user if data is out of accepted bounds
  } else {
    // accept the data and process
  }

  // 5d. Add any logic to navigate to other states
  smState = STATE_WAIT;
}
```


#### 6. Finally define the state machine function `sm_Run()`
_A switch case structure calls each function based on the current `smState` variable_
Note -> Automatically generate the switch case from the list of ENUM states and list of function prototypes! -> https://github.com/ImogenWren/switch-case-generator

```
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
```


### Modifying jsonConfig.h
 1. Define an enum to define variable types, These will be linked to a state enum so when a message with a keyword is received, we can look up what 
 data type will be included with the message
 ```
typedef enum {  // enum to pass variable types between functions
  EMPTY,
  INTEGER,
  FLOAT,
  CSTRING
} dataTypes;
```

1b. Define char string for the above enum to allow us to print out the enum above in a human readable format!
```
static char typeNames[][12] = {
  "EMPTY",
  "INTEGER",
  "FLOAT",
  "CSTRING"
};
```

2. Declare a list of all possible key values as ENUM. These values will be passed out of the jsonMessenger Object and can be used to control
a state machine seperate from this library.
_NOTE this list may not include all possible states, JUST the states that are triggered by receiving a command, including a null value at 0_

```
typedef enum {
  NONE,
  FAN_A0,
  FAN_A1,
  FAN_A2,
  FAN_A3,
  FAN_A4,
  FAN_B0,
  FAN_B1,
  FAN_B2,
  FAN_B3,
  FAN_B4,
  FAN_C0,
  FAN_C1,
  FAN_C2,
  FAN_C3,
  FAN_C4,
  FAN_D0,
  FAN_D1,
  FAN_D2,
  FAN_D3,
  FAN_D4,
  FAN_E0,
  FAN_E1,
  FAN_E2,
  FAN_E3,
  FAN_E4,
  ALL,
  MOD,
  USR,
  NUM_VALUES  // Add sentinal NUM_VALUES to count number of elements
} jsonStates;
```

2b. Then Declare a list of key commands that will be required to be parsed. This must match the order of the enums above, including the NULL or
NONE value, but does not have to include NUM_VALUES. This list will be the actual commands that can be parsed by the system
```
static char jsonCommandKeys[][5] = {
  "none",
  "A0", "A1", "A2", "A3", "A4",
  "B0", "B1", "B2", "B3", "B4",
  "C0", "C1", "C2", "C3", "C4",
  "D0", "D1", "D2", "D3", "D4",
  "E0", "E1", "E2", "E3", "E4",
  "all", "mod", "usr"
};
```
NOTE, this can also be used to turn the enums above back into strings for human readability, though this list is memory intensive and should 
not be copied outside of this library. Instead use functions within the library to print the values to the user if required.




3. Now Link each jsonState ENUM with the datatype ENUM in a map structure.
//In this example most will be integers, but will include some cstrings and to test.
```
const std::map<jsonStates, dataTypes> jsonStateMap = {
  { jsonStates::NONE, dataTypes::EMPTY },
  { jsonStates::FAN_A0, dataTypes::INTEGER },
  { jsonStates::FAN_A1, dataTypes::INTEGER },
  { jsonStates::FAN_A2, dataTypes::INTEGER },
  { jsonStates::FAN_A3, dataTypes::INTEGER },
  { jsonStates::FAN_A4, dataTypes::INTEGER },
  { jsonStates::FAN_B0, dataTypes::INTEGER },
  { jsonStates::FAN_B1, dataTypes::INTEGER },
  { jsonStates::FAN_B2, dataTypes::INTEGER },
  { jsonStates::FAN_B3, dataTypes::INTEGER },
  { jsonStates::FAN_B4, dataTypes::INTEGER },
  { jsonStates::FAN_C0, dataTypes::INTEGER },
  { jsonStates::FAN_C1, dataTypes::INTEGER },
  { jsonStates::FAN_C2, dataTypes::INTEGER },
  { jsonStates::FAN_C3, dataTypes::INTEGER },
  { jsonStates::FAN_C4, dataTypes::INTEGER },
  { jsonStates::FAN_D0, dataTypes::INTEGER },
  { jsonStates::FAN_D1, dataTypes::INTEGER },
  { jsonStates::FAN_D2, dataTypes::INTEGER },
  { jsonStates::FAN_D3, dataTypes::INTEGER },
  { jsonStates::FAN_D4, dataTypes::INTEGER },
  { jsonStates::FAN_E0, dataTypes::INTEGER },
  { jsonStates::FAN_E1, dataTypes::INTEGER },
  { jsonStates::FAN_E2, dataTypes::INTEGER },
  { jsonStates::FAN_E3, dataTypes::INTEGER },
  { jsonStates::FAN_E4, dataTypes::INTEGER },
  { jsonStates::ALL, dataTypes::INTEGER },
  { jsonStates::MOD, dataTypes::FLOAT },
  { jsonStates::USR, dataTypes::CSTRING }
};
```

4. Finally Declare a structure that will hold both the jsonStates enum, and any data that will need to be passed from jsonMessenger and outside of 
this library. <br>

We can make this fairly generic by including additional datatypes, or we can reduce the size of the memory used by removing the unneeded ones. <br>
This structure must include the jsonState, dataType and a `command_received` bool that can be used to flag that a new command has been received to methods 
outside of the library. All other data is arbitary and will depend on use case, though the library has been built to accomidate those listed. 
```
struct jsonStateData {
  jsonStates cmdState;        // The command state enum to tell state machine what state to go to next
  dataTypes data_type;        // The type of data being passed along with structure (though state should know what data to expect anyway), this could be removed to save space
  int numeric;                // empty generic data slots for each data type
  float data;
  char msg[16];
  bool cmd_received;           // Flag set true by jsonLoop when cmd is received
};
```

<br><br>

With these program structures filled in for the specific use case, we can look at the program outside of the library and how it can be used!
