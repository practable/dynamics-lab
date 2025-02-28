/*  Dynamics Lab Firmware V0.0.0

Written By:
Imogen Heard
32/01/2025



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


*/




#include "globals.h"







void setup() {
  Serial.begin(115200);
  delay(2000);  // give time for Serial object to start
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
  // replaced with arafruit library
  //  mpu.Initialize();
  //  mpu.RegisterWrite(MPU6050_ACCEL_CONFIG, 0b00011000);  // It does this in Initialize, set to 0x08 (+-4G)
  //  mpu.Calibrate();
  mpu_setup();
  stepper_setup(false);
  servo.attach(SERVO_PPM_PIN, SERVO_ZERO_uS);  // default width is hopefully at one end of travel  -> moving this function to the "ping" state to try and avoid chattering (this doesnt work, but may be a good reason to use servoBasic lib instead)

  // display_mallinfo();
  servo_pos = false;
  //  delay(1000);
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
  if (sampleDelay.millisDelay(sampleDelay_mS)) {
    if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the
      encode_array[samples_written] = stepper.encoder.getAngle();
      accX_array[samples_written] = a.acceleration.x / G_CONST;  //mpu.GetAccX();
      accY_array[samples_written] = a.acceleration.y / G_CONST;  //mpu.GetAccY();
      accZ_array[samples_written] = a.acceleration.z / G_CONST;  //mpu.GetAccZ();
      gyroX_array[samples_written] = g.gyro.x;                   // mpu.GetGyroX();
      gyroY_array[samples_written] = g.gyro.y;                   //mpu.GetGyroY();
      gyroZ_array[samples_written] = g.gyro.z;                   //mpu.GetGyroZ();
      samples_written++;
    } else if (samples_written == DATA_ARRAY_SIZE) {
      // we have written our last sample to the array, if it was a string, would append with a /n
      // but as floats this isnt needed
    }
  }

  // do streaming data at the specified rate
  if (streaming_active || snapshop_active) {
    if (printDelay.millisDelay(print_delay_mS)) {
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

  errors.clear_warning();  // clear JSON (move this to bottom of loop later)
                           //  if (printDelay.millisDelay(20000)) {
                           // std::cout << "alive" << std::endl;
                           //Serial.println("Alive and Loop");
                           // display_mallinfo();
                           //  stepper.setRPM(0);
                           //}
}
