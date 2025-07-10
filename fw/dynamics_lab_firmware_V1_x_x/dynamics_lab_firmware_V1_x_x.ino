/*  Dynamics Lab Firmware V0.2.0
_Control a uStepperS32 controlled stepper motor, set velocity in Hz or RPM, and stream data from onboard accellerometer_

- uStepperS32 uses `STM32F401` MCU Controller


Written By:
Imogen Heard
03/03/2025

NOTE: This requires modification of the uStepper32 library files!
 for correct fork see ->>>>>>>>>> https://github.com/ImogenWren/uStepperS32

 Specific change: TLE5012B.cpp, comment out lines in init function
 ```
//this->encoderOffset = this->angle;
//this->angle = 0;
 ```
 Also Changed: UstepperS32.cpp

  commented out line:
  ```
//encoder.setHome(); // Changed Imogen Heard 28/02/25 
  ```
in setup function

ALSO:
- added new function to library encoder.setHomeActual(uint16_t encoderVal)
run this function and pass current encoder value when encoder is at zero position

*/

/* Version Control 
Version V0.0.0 Alpha Prototype

Version V0.1.0 Alpha Prototype with standardised command structure
Sketch uses 72264 bytes (27%) of program storage space. Maximum is 262144 bytes.
Global variables use 7524 bytes (11%) of dynamic memory, leaving 58012 bytes for local variables. Maximum is 65536 bytes.
- New command structure working `{"set":"{CMD},"to":{VALUE}"}`

Version V0.1.1
Sketch uses 71812 bytes (27%) of program storage space. Maximum is 262144 bytes.
Global variables use 7524 bytes (11%) of dynamic memory, leaving 58012 bytes for local variables. Maximum is 65536 bytes.
- Lots of small updates to standardise all debugging outputs into correct JSON formatting

Version V0.2.0
- Need to update homing algorithm to use 2 point calibration
- Changed accellerometer library to +- 4G maximum sensor reading
- added/modified sample-rate and print rate functions to save data to array for greater time resolution
Sketch uses 74380 bytes (28%) of program storage space. Maximum is 262144 bytes.
Global variables use 12600 bytes (19%) of dynamic memory, leaving 52936 bytes for local variables. Maximum is 65536 bytes.

Version V0.2.1
Sketch uses 76296 bytes (29%) of program storage space. Maximum is 262144 bytes.
Global variables use 29856 bytes (45%) of dynamic memory, leaving 35680 bytes for local variables. Maximum is 65536 bytes.
- Now library has been modified for absolute encoder position, 
    - Adding ability to save encoder offset value in EEPROM
- modified encoder.setHomeActual -> added function to manually reset the encoder angle

Version V0.2.2
- Updated sampling timing for more even timing between samples

Version V0.2.3
Sketch uses 76952 bytes (29%) of program storage space. Maximum is 262144 bytes.
Global variables use 29864 bytes (45%) of dynamic memory, leaving 35672 bytes for local variables. Maximum is 65536 bytes.
- Changed "home" state to non-blocking
- Change GOTO state to work using encoder externally -> needs more work but is in progress


*/




#include "globals.h"







void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);  // give time for Serial object to start
  }

  // delay(2000);  // give time for Serial object to start
  // display_mallinfo();
  //  std::cout << "\n{\"model\":\"" << EXPERIMENT_NAME << "\",\"version\":\"" << FIRMWARE_VERSION << "\",\"developed-by\":\"" << DEVELOPER << "\"}" << std::endl;
  Serial.print("\n{\"model\":\"");
  Serial.print(EXPERIMENT_NAME);
  Serial.print("\",\"version\":\"");
  Serial.print(FIRMWARE_VERSION);
  Serial.print("\",\"fw-developed-by\":\"");
  Serial.print(DEVELOPER);
  Serial.println("\"}");
  //  Serial.print("{\"no_samples_in_array\": \"");
  //  Serial.print(num_samples_req);
  //  Serial.println("\"}");
  jsonRX.jsonBegin();  // Start the json library to accept commands over serial connection
  // replaced with adafruit library
  //  mpu.Initialize();
  //  mpu.RegisterWrite(MPU6050_ACCEL_CONFIG, 0b00011000);  // It does this in Initialize, set to 0x08 (+-4G)
  //  mpu.Calibrate();
  mpu_setup();
  get_offset_from_memory();  // get encoder_offset from persistant memory before stepper setup (does nothing atm)
  stepper_setup(false);      // if true run old homing scripts



  //servo.attach(SERVO_PPM_PIN, SERVO_ZERO_uS);  // default width is hopefully at one end of travel  -> moving this function to the "ping" state to try and avoid chattering (this doesnt work, but may be a good reason to use servoBasic lib instead)

  stepper.encoder.setHomeActual(ENCODER_HOME_OFFSET);

  // display_mallinfo();
  servo_pos = false;
  // fix for resetting servo at startup
  servo.attach(SERVO_PPM_PIN, SERVO_ZERO_uS);
  servo_attach_time_mS = millis();
  servo.writeMicroseconds(SERVO_OPEN_uS);
  servo_pos = true;
  //sm_state_ping();   // make sure servo is homed correctly
  //  delay(1000);
  pinMode(LED_BEACON, OUTPUT);
  digitalWrite(LED_BEACON, true);
}



void loop() {


  jsonStateData nextState_data = jsonRX.jsonReadSerialLoop();


  if (nextState_data.cmd_received) {  // If command is receive   //delay(10);



    const char* cmd = jsonRX.getCMDkey(nextState_data.cmdState);  // I feel like the entire point of using ENUMs is being totally lost by doing this, but it is working
    //std::cout << std::endl;
    // std::cout << "{\"rx-cmd\":\"" << cmd << "\",\"datatype\":\"" << jsonRX.getDataType(nextState_data.data_type) << "\",\"data\":\"";
    Serial.print("{\"rx-cmd\":\"");
    Serial.print(cmd);
    Serial.print("\",\"datatype\":\"");
    Serial.print(jsonRX.getDataType(nextState_data.data_type));
    Serial.print("\",\"data\":\"");
    if (nextState_data.data_type == INTEGER) Serial.print(nextState_data.numeric);  //std::cout << nextState_data.numeric;
    if (nextState_data.data_type == FLOAT) Serial.print(nextState_data.floatData);  //std::cout << nextState_data.floatData;
    if (nextState_data.data_type == CSTRING) Serial.print(nextState_data.msg);      //std::cout << nextState_data.msg;
    if (nextState_data.data_type == EMPTY) Serial.print("n/a");                     //std::cout << "n/a";
    // Is this now missing float clause?
    //std::cout << "\"}" << std::endl;
    Serial.println("\"}");

    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState_data.cmdState == STOP) {  // if fan speed change command received
      smState = STATE_STOP;
    } else if (nextState_data.cmdState == START) {
      smState = STATE_START;
    } else if (nextState_data.cmdState == SET_SPEED_HZ) {
      smState = STATE_SET_SPEED_HZ;
    } else if (nextState_data.cmdState == SET_SPEED_RPM) {
      smState = STATE_SET_SPEED_RPM;
    } else if (nextState_data.cmdState == HOME) {
      smState = STATE_HOME;
    } else if (nextState_data.cmdState == CALIBRATE) {
      smState = STATE_CALIBRATE;
    } else if (nextState_data.cmdState == FREEWHEEL) {
      smState = STATE_FREEWHEEL;
    } else if (nextState_data.cmdState == BRAKE) {
      smState = STATE_BRAKE;
    } else if (nextState_data.cmdState == GOTO) {
      smState = STATE_GOTO;
    } else if (nextState_data.cmdState == SAMPLERATE) {
      smState = STATE_SAMPLERATE;
    } else if (nextState_data.cmdState == PRINTRATE) {
      smState = STATE_PRINTRATE;
    } else if (nextState_data.cmdState == STARTSTREAM) {
      smState = STATE_STARTSTREAM;
    } else if (nextState_data.cmdState == STOPSTREAM) {
      smState = STATE_STOPSTREAM;
    } else if (nextState_data.cmdState == SNAPSHOT) {
      smState = STATE_SNAPSHOT;
    } else if (nextState_data.cmdState == SNAPTIME) {
      smState = STATE_SNAPTIME;
    } else if (nextState_data.cmdState == PING) {
      smState = STATE_PING;
    } else if (nextState_data.cmdState == OFFSET) {
      smState = STATE_OFFSET;
    } else if (nextState_data.cmdState == HELP) {
      smState = STATE_HELP;
    } else {
      // std::cout << "{\"WARNING\":\"Unrecognised cmdState\"}" << std::endl;
      Serial.println("{\"WARNING\":\"Unrecognised cmdState\"}");
    }
  }


  sm_Run(nextState_data);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command
  // NOTE System design question here, passing this value as a local variable means it cannot be updated elsewhere. Making it global means that other states would be able to modify the data
  // and pass it internally.
  // Skipping over this question, I am going to do the thing this would solve a different way
  // sm_Run();

  // mpu.Execute();


  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Do sampling Data at the specified rate
  if (sampleDelay.millisDelay(sampleDelay_mS)) {                                   // added OR if samples written has been reset to 0 after printing || samples_written == 0
    if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the
      timestamp_array[samples_written] = millis();
      encode_array[samples_written] = stepper.encoder.getAngle();
      accX_array[samples_written] = ((a.acceleration.x / G_CONST) - acc_offset.X);  // - acc_offset.X;  //mpu.GetAccX();
      accY_array[samples_written] = ((a.acceleration.y / G_CONST) - acc_offset.Y);  /// - acc_offset.Y;  //mpu.GetAccY();
      accZ_array[samples_written] = ((a.acceleration.z / G_CONST) - acc_offset.Z);  // Added 0.3 offset due to sensor calibration issue  // - acc_offset.Z;  //mpu.GetAccZ();
      gyroX_array[samples_written] = g.gyro.x - gyro_offset.X;                      // mpu.GetGyroX();
      gyroY_array[samples_written] = g.gyro.y - gyro_offset.Y;                      //mpu.GetGyroY();
      gyroZ_array[samples_written] = g.gyro.z - gyro_offset.Z;                      //mpu.GetGyroZ();
      samples_written++;
    }
    // else if (samples_written == DATA_ARRAY_SIZE) {
    // we have written our last sample to the array, if it was a string, would append with a /n
    // but as floats this isnt needed
    // }
  }

  // do streaming data at the specified rate
  if (streaming_active || snapshop_active) {
    // if (printDelay.millisDelay(print_delay_mS)) {
    if (samples_written >= num_samples_req) {  // REMOVED PRINT TIMER
      sampleDelay.resetDelayTime_mS();         // makes sure that the sample loop is synced to the printing loop //moved to try and improve timings (doing this first so next sample is sooner)
      //print the sampled data
      update_json(samples_written);
      samples_written = 0;
    }
    if (snapshop_active) {
      if (millis() - snapshot_starttime_mS >= snapshot_timer_mS) {
        snapshop_active = false;
      }
    }
  }

  // Fix for servo chattering, servo is attached during PING state
  if (servo.attached()) {
    if (millis() - servo_attach_time_mS >= SERVO_TIMEOUT_mS) {
      servo.detach();
    }
  }

  errors.clear_warning();  // clear JSON (move this to bottom of loop later)
                           //  if (printDelay.millisDelay(20000)) {
                           // std::cout << "alive" << std::endl;
                           //Serial.println("Alive and Loop");
                           // display_mallinfo();
                           //  stepper.setRPM(0);
                           //}
}
