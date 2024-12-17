
#include <UstepperS32.h>
UstepperS32 stepper;
#include "TinyMPU6050.h"
MPU6050 mpu;
#include <autoDelay.h>
autoDelay printDelay;

#define PRINT_RATE_Hz 2
#define PRINT_PERIODIC_UPDATES true

#define HALL_SENSOR_PIN A4
#define HALL_SENSOR_LOW_TRIGGER 295   // home state is triggered when hall sensor returns lower than this value
#define HALL_SENSOR_HIGH_TRIGGER 700  // estimate for backwards magnet

#define HALL_NORMALLY_HIGH true  // define if normally high, triggered by low pulse (true) or normally low triggered by high pulse (false)

#define ENCODE_RAW_ANGLE_OFFSET 0.0
#define STEPPER_HOLD_CURRENT 0

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




// do one full revolution to find the lowest value reported by the hall effect sensor
void find_home() {
  Serial.println("Finding Home");

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
      Serial.println("ending init rotation");
      finding_center = false;
    }
  }
}

// Use the previously found hall effect sensor low reading to return the mass to the center position
void move_home() {
  bool home_found = false;
  int16_t hall_sensor_val;
  Serial.println("Moving Home, please stand by..");
  while (!home_found) {
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);
    //  Serial.println(hall_sensor_val);
    if (hall_sensor_val == low_point) {
      stepper.stop();
      home_found = true;
      Serial.print("Home Found, Angle: ");
      Serial.println(stepper.encoder.getAngleRaw());
      stepper.moveAngle(180);  // this method seems to be under performing by factor of 10!
      while (stepper.getMotorState(POSITION_REACHED)) {
        // Serial.println("Moving to Positon");
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
}

void setup() {
  Serial.begin(115200);
  delay(100);  // for stability
  Serial.println("\nuStepper S32 - Test");
  // delay(1000);  // for UI

  mpu.Initialize();                                                // Initialization of MPU
  mpu.Calibrate();                                                 // Calibration - LED will blink until calibration is done !
                                                                   // delay(3000);                                                     // for calibration
  stepper.setup(NORMAL, 400, 10, 0.2, 0.0, 16, true, false, 100, 1);  //Initialize uStepper S32
  //stepper.setCurrent(100);     // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(STEPPER_HOLD_CURRENT);  // set holding current as percentage

  //  stepper.enableStallguard(0, false);  // (threshold (-64 to + 63), stop_on_stall, RPM?) // default 4  // this is useless

  find_home();
  move_home();


  Serial.print("Current Encoder Pos (raw): ");
  Serial.println(stepper.encoder.getAngleRaw());

  Serial.print("Current Encoder Pos cooked: ");
  Serial.println(round(stepper.encoder.getAngle()));



  stepper.stop();
  Serial.print("To Run Motor, enter speed in Hz between -20, 0 and ~20\n");
}


//setBrakeMode(uint8_t mode, float brakeCurrent = 25.0);


#define COMMAND_SIZE 64
//char command[COMMAND_SIZE];


float rpm = 0;
float Hz = 0;

uint32_t last_rotation_mS;
uint32_t current_rotation_mS;

uint16_t last_encoder_pos = 0;  // used to independently detect stall

bool ping_mode = true;  // temporary flag to keep data output while doing a servo "ping"

bool stall_detected = false;
uint16_t stallCount = 0;

#define STALL_COUNT_LIMIT 5

void loop() {
  //char cmd;

  // Get Serial Message
  if (Serial.available() > 0) {
    float value;
    char command[COMMAND_SIZE];
    Serial.readBytesUntil(10, command, COMMAND_SIZE);
    Serial.print("\ncmd: ");
    Serial.println(command);




    value = atof(command);
    Serial.print("data: ");
    Serial.println(value);

    // Parse command
    if (value == 0) {
      stepper.stop(HARD);
      stepper.setRPM(0);
      Serial.println("stop cmd received");
      Hz = 0;
      rpm = 0;
      move_home();
    } else if (value > 0 || value < 0) {
      Hz = value;
      rpm = get_RPM_from_Hz(Hz);
      stepper.setRPM(rpm*-1);            // invert movement so clockwise is positive 
    }
  }

  float encode_rpm = stepper.encoder.getRPM();
  float encode_pos = stepper.encoder.getAngle();
  uint16_t current_encoder_pos = stepper.encoder.getAngleRaw();
  if (current_encoder_pos == last_encoder_pos && rpm != 0) {
    stallCount++;
    if (stallCount > 1) {
      Serial.print("Motor Stall Detected No: ");
      Serial.println(stallCount);
    }
    if (stallCount >= STALL_COUNT_LIMIT) {
      stepper.stop(HARD);
      stepper.setRPM(0);
      Hz = 0;
      rpm = 0;
      //find_home();   // see if actually required
      move_home();
      stallCount = 0;
    }
  } else {
    stallCount = 0;
  }
  last_encoder_pos = current_encoder_pos;

  // Independent Timing Check



  // Interlocks/safety code
  // if (stepper.isStalled()) {
  //   Serial.println("Motor Has Stalled, stopping movement & re-calibrating");
  //   stepper.stop(HARD);
  //    delay(2000);
  //    stepper.clearStall();
  //    delay(100);
  //    find_home();
  //    move_home();
  //    stepper.setBrakeMode(FREEWHEELBRAKE);
  //    Hz = 0;
  //    rpm = 0;
  // }



  // Gather Accellerometer data
  mpu.Execute();

  // float Xacc = mpu.GetAccX();
  // Program Text Output

  if (rpm > 0.0 || rpm < 0.0 || ping_mode) {  // If the motor has commanded movement, or ping mode is active
    if (printDelay.millisDelay(print_delay_mS)) {
      // if (Xacc == 0){
      // float actualRPM = stepper.getDriverRPM();
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
      dtostrf(mpu.GetAccX(), 1, 1, accX_buf);
      dtostrf(mpu.GetAccY(), 1, 1, accY_buf);
      dtostrf(mpu.GetAccZ(), 1, 1, accZ_buf);
      dtostrf(mpu.GetGyroX(), 1, 1, gyroX_buf);
      dtostrf(mpu.GetGyroY(), 1, 1, gyroY_buf);
      dtostrf(mpu.GetGyroZ(), 1, 1, gyroZ_buf);

      Serial.println(accX_buf);


      dtostrf(Hz, 3, 3, hz_buf);
      dtostrf(rpm, 3, 3, rpm_buf);

      dtostrf(actualHz, 3, 3, a_hz_buf);
      dtostrf(encode_rpm, 3, 3, a_rpm_buf);

      sprintf(printBuffer, "pos: %5s ,set_Hz: %4s, rep_Hz: %4s, set_RPM: %4s, rep_rpm: %4s, acc:%4s,%4s,%4s, gyro:%4s,%4s,%4s ", pos_buf, hz_buf, a_hz_buf, rpm_buf, a_rpm_buf, accX_buf, accY_buf, accZ_buf, gyroX_buf, gyroY_buf, gyroZ_buf);
#if PRINT_PERIODIC_UPDATES == true
      Serial.println(printBuffer);
#endif
    }
  }
}
