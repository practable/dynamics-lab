
#include <UstepperS32.h>
UstepperS32 stepper;
#include "TinyMPU6050.h"
MPU6050 mpu;
#include <autoDelay.h>
autoDelay printDelay;
autoDelay stallDelay;

#define PRINT_RATE_Hz 40
#define PRINT_PERIODIC_UPDATES true

#define HALL_SENSOR_PIN A4
//#define HALL_SENSOR_LOW_TRIGGER 295   // home state is triggered when hall sensor returns lower than this value
//#define HALL_SENSOR_HIGH_TRIGGER 700  // estimate for backwards magnet

#define HALL_NORMALLY_HIGH true  // define if normally high, triggered by low pulse (true) or normally low triggered by high pulse (false)

#define ENCODE_RAW_ANGLE_OFFSET 0.0
#define STEPPER_HOLD_CURRENT 10

#define COMMAND_SIZE 64

// this is definatly better done as a timer
// Stall count limit never reaches above 5 as position changes just enough to clear stall warning
#define STALL_COUNT_LIMIT 5  // Limit for typical number of stall events before triggering stall reset behaviour
// Variables to track total number of stalls and limit users to a defined number per time period
#define STALL_OPPORTUNITIES 3      // number of times stall reset behaivour can be triggered before motor is limited
#define STALL_COOL_DOWN_PERIOD 15  // cool down period to reset the number of triggered stalls
// When limit is reached, automatic stall guard is implemented by limiting how long the motor can stay in stall condition
#define PROTECT_STALL_COUNT_LIMIT 2  // Limit for number of stall events that trigger stall reset behaviour if protection mode has been activated

#define MAX_MOTOR_STEPS_S 800
#define MAX_MOTOR_ACC_STEPS_S_S 4000

bool ping_mode = false;  // temporary flag to keep data output while doing a servo "ping"

uint32_t print_delay_mS = 1000 / PRINT_RATE_Hz;

float get_RPM_from_Hz(float Hz) {
  float rpm = Hz * 60.0;
  return rpm;
}


bool calibrated = false;
float cal_plus_angle;
float cal_minus_angle;

float cal_plus_acc = 0;
float cal_minus_acc = 0;

// used in homing algorithm
int16_t low_point;
float low_angle;

//setBrakeMode(uint8_t mode, float brakeCurrent = 25.0);



//char command[COMMAND_SIZE];


float rpm = 0;
float Hz = 0;

uint32_t last_rotation_mS;
uint32_t current_rotation_mS;

uint16_t last_encoder_pos = 0;  // used to independently detect stall



// Variables to keep track of a stall as it happens
bool stall_detected = false;
uint16_t stallCount = 0;





int16_t stall_total = 0;

uint16_t current_stall_limit = STALL_COUNT_LIMIT;  // this variable is used by check stall status and updated by  stall_manager();
// Maybe this is better implemented as a timer rather than a count





// do one full revolution to find the lowest value reported by the hall effect sensor
void find_home() {
  // Serial.println("Finding Home");

  int16_t hall_sensor_val;
  bool finding_center = true;
  low_point = (uint16_t)~0 >> 1;  // set low-pouint to maximum value (could hard code high value but this is more fun)

  stepper.setMaxVelocity(800);
  stepper.setMaxAcceleration(4000);
  hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // get these up front
  low_angle = stepper.encoder.getAngleRaw();
  stepper.moveAngle(365);


  while (finding_center) {
    // Serial.print("angle moved: ");
    // Serial.println(stepper.angleMoved());
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);
    if (hall_sensor_val <= low_point) {
      low_point = hall_sensor_val;
      Serial.print("New Low Point Recorded: ");
      Serial.print(low_point);
      low_angle = stepper.encoder.getAngleRaw();
      Serial.print(" angle: ");
      Serial.println(low_angle);
    }
    if (stepper.angleMoved() >= 360 || stepper.angleMoved() <= -360) {
      //     Serial.println("ending init rotation");
      finding_center = false;
    }
  }
}

#define HOMING_TIMEOUT_S 10
// Use the previously found hall effect sensor low reading to return the mass to the center position
int16_t move_home() {
  uint32_t start_time_mS = millis();
  bool home_found = false;
  int16_t hall_sensor_val;
  //  Serial.println("Moving Home, please stand by..");
  while (!home_found) {
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // measure the hall sensor
                                                    // Serial.println(hall_sensor_val);
    // timeout clause here to exit loop if home cannot be found (two functions can be written into their own loop with the output of this function as the escape clause)
    if (millis() - start_time_mS >= HOMING_TIMEOUT_S * 1000) {
      Serial.println("move_home() timeout");
      /// Serial.println("Moving to Angle 0");
      //stepper.moveToAngle(0);  //
      return 0;
    }

    if (hall_sensor_val == low_point) {  // when the low point is found again, run homing sequence
      stepper.stop();
      home_found = true;
      Serial.print("Home Found, Angle: ");
      Serial.println(stepper.encoder.getAngleRaw());
      stepper.moveAngle(180);  // this method seems to be under performing by factor of 10!
      while (stepper.getMotorState(POSITION_REACHED)) {
        ;  // wait for motor to reach position
      }
      stepper.encoder.setHome();
      Serial.print("Resetting Home, Angle: ");
      Serial.println(stepper.encoder.getAngleRaw());
      stepper.stop();
      stepper.setBrakeMode(COOLBRAKE);
      delay(100);
    } else {
      //stepper.moveSteps(10);
      stepper.setRPM(60);
    }
  }
  return 1;  // on success return 1
}

// wraps previous two functions together into a reliable structure
void run_calibration() {
  bool cal_complete;
  while (!cal_complete) {
    find_home();
    cal_complete = move_home();
  }
  stepper.setMaxVelocity(MAX_MOTOR_STEPS_S);
  stepper.setMaxAcceleration(MAX_MOTOR_ACC_STEPS_S_S);
}

//float encode_rpm;
//float encode_pos;
// Function to track stall status
void check_stall_status() {
  float encode_rpm = stepper.encoder.getRPM();
  float encode_pos = stepper.encoder.getAngle();
  uint16_t current_encoder_pos = stepper.encoder.getAngleRaw();
  if (current_encoder_pos == last_encoder_pos && rpm != 0) {
    stallCount++;
    if (stallCount > 2) {
      Serial.print("Motor Stall Detected No: ");  // detection of momentary stall does not trigger stall protection behaviour
      Serial.println(stallCount);
    }
    if (stallCount >= current_stall_limit) {  // If several stalls detected then stall protection kicks in
      stepper.stop(HARD);
      stepper.setRPM(0);
      Hz = 0;
      rpm = 0;
      //find_home();   // see if actually required
      Serial.println("Stall Limit Reached, Motor Resetting");
      if (stall_total >= STALL_OPPORTUNITIES) {
        Serial.println("Too many stalls, stall limitation applied");
      }
      move_home();
      stallCount = 0;
      stall_total++;  // add one to the total stalls
    }
  } else {
    stallCount = 0;
  }
  last_encoder_pos = current_encoder_pos;
}


// Keeps track of how many stalls have been triggered over the cool down period and limits the length of time allowed for a stall if limit has been reached
//
void stall_manager() {
  if (stall_total >= STALL_OPPORTUNITIES) {           // If too many stalls have been triggered
    current_stall_limit = PROTECT_STALL_COUNT_LIMIT;  // set the stall limit to the protected leve
  } else {
    current_stall_limit = STALL_COUNT_LIMIT;  // else set to max level
  }
  if (stallDelay.minutesDelay(STALL_COOL_DOWN_PERIOD)) {  // If cool down period has elapsed
    if (stall_total > 0) {                                // If stalls have happened
      stall_total--;                                      // Remove one stall_total every cool-down period
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);  // for stability
  Serial.println("\nuStepper S32 - Test");
  delay(1000);  // for UI

  mpu.Initialize();                                                   // Initialization of MPU
  mpu.Calibrate();                                                    // Calibration - LED will blink until calibration is done !
                                                                      // delay(3000);                                                     // for calibration
  stepper.setup(NORMAL, 400, 10, 0.2, 0.0, 16, true, false, 100, 1);  //Initialize uStepper S32
  stepper.setCurrent(100);                                            // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(STEPPER_HOLD_CURRENT);                       // set holding current as percentage

  // stepper.enableStallguard(0, false);  // (threshold (-64 to + 63), stop_on_stall, RPM?) // default 4  // this is useless

  run_calibration();

  Serial.print("Current Encoder Pos (raw): ");
  Serial.println(stepper.encoder.getAngleRaw());

  Serial.print("Current Encoder Pos cooked: ");
  Serial.println(round(stepper.encoder.getAngle()));



  stepper.stop();
  Serial.print("To Run Motor, enter speed in Hz between -20, 0 and ~20\n");
}





void loop() {
  //char cmd;

  // Get Serial Message
  if (Serial.available() > 0) {
    float value;
    char command[COMMAND_SIZE];
    Serial.readBytesUntil(10, command, COMMAND_SIZE);
    Serial.print("\ncmd: ");
    Serial.println(command);
    Serial.println();




    value = atof(command);
    Serial.print("data: ");
    Serial.println(value);

    // Parse command

    if (value == -100) {  // hack case to trigger a servo ping

    } else if (value == 0) {
      stepper.stop(HARD);
      stepper.setRPM(0);
      //   Serial.println("stop cmd received");
      Hz = 0;
      rpm = 0;
      //move_home();
      run_calibration();
    } else if (value > 0 || value < 0) {
      Hz = value;
      rpm = get_RPM_from_Hz(Hz);
      stepper.setRPM(rpm * -1);  // invert movement so clockwise is positive
    }
  }


  // Function to track stall status
  // check_stall_status();

  // Function to track total stalls per period
  // stall_manager();






  // Gather Accellerometer data
  mpu.Execute();

  // float Xacc = mpu.GetAccX();
  // Program Text Output

  if (rpm > 0.0 || rpm < 0.0 || ping_mode) {  // If the motor has commanded movement, or ping mode is active
 //   if (printDelay.millisDelay(print_delay_mS)) {
if (printDelay.secondsDelay(5)) {
      // if (Xacc == 0){
      // float actualRPM = stepper.getDriverRPM();
      float encode_rpm = stepper.encoder.getRPM();
      float encode_pos = stepper.encoder.getAngle();
      float actualHz = encode_rpm / 60.0;
      char printBuffer[200];
      char pos_buf[8];
      char hz_buf[8];
      char rpm_buf[8];
      char a_hz_buf[8];
      char a_rpm_buf[8];

      char accX_buf[8];
      char accY_buf[8];
      char accZ_buf[8];

      char gyroX_buf[8];
      char gyroY_buf[8];
      char gyroZ_buf[8];

      dtostrf(encode_pos, 1, 1, pos_buf);
      // dtostrf(Xacc, 2, 2, accX_buf); //mpu.GetAccX()
      dtostrf(mpu.GetAccX(), 2, 2, accX_buf);
      dtostrf(mpu.GetAccY(), 2, 2, accY_buf);
      dtostrf(mpu.GetAccZ(), 2, 2, accZ_buf);
      dtostrf(mpu.GetGyroX(), 2, 2, gyroX_buf);
      dtostrf(mpu.GetGyroY(), 2, 2, gyroY_buf);
      dtostrf(mpu.GetGyroZ(), 2, 2, gyroZ_buf);

      // Serial.println(accX_buf);


      dtostrf(Hz, 2, 2, hz_buf);
      dtostrf(rpm, 2, 2, rpm_buf);

      dtostrf(actualHz, 2, 2, a_hz_buf);
      dtostrf(encode_rpm, 2, 2, a_rpm_buf);

      sprintf(printBuffer, "pos: %6s ,set_Hz: %6s, rep_Hz: %6s, set_RPM: %6s, rep_rpm: %6s, acc: %6s, %6s, %6s, gyro: %6s, %6s, %6s ", pos_buf, hz_buf, a_hz_buf, rpm_buf, a_rpm_buf, accX_buf, accY_buf, accZ_buf, gyroX_buf, gyroY_buf, gyroZ_buf);
      //sprintf(printBuffer, "%6s, %6s, %6s, %6s, %6s, %6s, %6s ", pos_buf, accX_buf, accY_buf, accZ_buf, gyroX_buf, gyroY_buf, gyroZ_buf);
      // sprintf(printBuffer, "%6s, %6s, %6s,", accX_buf, accY_buf, accZ_buf);
#if PRINT_PERIODIC_UPDATES == true
        Serial.println(printBuffer);
#endif
    }
  }
}
