
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


// do one full revolution then go back to lowest val
void find_home() {
  bool home_found = false;
  int16_t hall_sensor_val;
  while (!home_found) {
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);
    Serial.println(hall_sensor_val);
    if (hall_sensor_val < HALL_SENSOR_LOW_TRIGGER) {
      stepper.stop();
      home_found = true;
      stepper.encoder.setHome();
      delay(100);
    } else {
      //stepper.moveSteps(10);
      stepper.setRPM(120);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // for stability
  Serial.println("\nuStepper S32 - Test");
  delay(1000);  // for UI

  mpu.Initialize();                                                // Initialization of MPU
  mpu.Calibrate();                                                 // Calibration - LED will blink until calibration is done !
  delay(3000);                                                     // for calibration
  stepper.setup(NORMAL, 400, 10, 0.2, 0.0, 16, false, 0, 50, 30);  //Initialize uStepper S32
  //stepper.setCurrent(100);     // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(1);  // set holding current as percentage

  find_home();

  Serial.print("Current Encoder Pos (raw): ");
  Serial.println(stepper.encoder.getAngleRaw());

  Serial.print("Current Encoder Pos cooked: ");
  Serial.println(stepper.encoder.getAngle());



  stepper.stop();
  Serial.print("To Run Motor, enter speed in Hz between 0 and ~20\n");
}


//setBrakeMode(uint8_t mode, float brakeCurrent = 25.0);


#define COMMAND_SIZE 64
//char command[COMMAND_SIZE];


float rpm = 0;
float Hz = 0;

uint32_t last_rotation_mS;
uint32_t current_rotation_mS;

void loop() {
  //char cmd;

  // Get Serial Message
  if (Serial.available() > 0) {
    float value;
    char command[COMMAND_SIZE];
    Serial.readBytesUntil(10, command, COMMAND_SIZE);
    Serial.print("cmd: ");
    Serial.println(command);




    value = atof(command);
    Serial.print("data: ");
    Serial.println(value);

    // Parse command
    if (value == 0) {
      stepper.stop(HARD);
      Serial.println("stop cmd received");
      Hz = 0;
      rpm = 0;
      //stepper.moveToAngle(0.0);
    } else if (value > 0 || value < 0) {
      Hz = value;
      rpm = get_RPM_from_Hz(Hz);
      stepper.setRPM(rpm);
    }
  }

  float encode_rpm = stepper.encoder.getRPM();
  float encode_pos = stepper.encoder.getAngle();

  // Independent Timing Check



  // Interlocks/safety code
  //  if (stepper.isStalled()) {
  //    Serial.println("Motor Has Stalled");
  //    stepper.setBrakeMode(FREEWHEELBRAKE);
  //    Hz = 0;
  //    rpm = 0;
  //  }

  // Gather Accellerometer data
  mpu.Execute();

  float Xacc = mpu.GetAccX();
  // Program Text Output
  if (rpm > 0.0 || rpm < 0.0) {
    if (printDelay.millisDelay(print_delay_mS)) {
      // if (Xacc == 0){
      // float actualRPM = stepper.getDriverRPM();
      float actualHz = encode_rpm / 60.0;
      char printBuffer[128];
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
      dtostrf(Xacc, 1, 1, accX_buf);
      dtostrf(mpu.GetAccY(), 1, 1, accY_buf);
      dtostrf(mpu.GetAccZ(), 1, 1, accZ_buf);
      dtostrf(mpu.GetGyroX(), 1, 1, gyroX_buf);
      dtostrf(mpu.GetGyroY(), 1, 1, gyroY_buf);
      dtostrf(mpu.GetGyroZ(), 1, 1, gyroZ_buf);



      dtostrf(Hz, 3, 3, hz_buf);
      dtostrf(rpm, 3, 3, rpm_buf);

      dtostrf(actualHz, 3, 3, a_hz_buf);
      dtostrf(encode_rpm, 3, 3, a_rpm_buf);

      sprintf(printBuffer, "pos: %6s ,set_Hz: %6s, rep_Hz: %6s, set_RPM: %6s, rep_rpm: %6s, acc:%6s,%6s,%6s, gyro:%6s,%6s,%6s ", pos_buf, hz_buf, a_hz_buf, rpm_buf, a_rpm_buf, accX_buf, accY_buf, accZ_buf, gyroX_buf, gyroY_buf, gyroZ_buf);
#if PRINT_PERIODIC_UPDATES == true
      Serial.println(printBuffer);
#endif
    }
  }
}
