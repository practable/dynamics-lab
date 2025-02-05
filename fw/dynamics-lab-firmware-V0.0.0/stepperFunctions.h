/* stepperFunctions.h

 Any functions that relate to the operation of the uStepper32 stepper motor

*/

#pragma once

float get_RPM_from_Hz(float Hz) {
  float rpm = Hz * 60.0;
  return rpm;
}

float get_Hz_from_RPM(float rpm) {
  float Hz = rpm/60.0;
  return Hz;
}


// do one full revolution to find the lowest value reported by the hall effect sensor
void step_find_home() {
  // Serial.println("Finding Home");

  int16_t hall_sensor_val;
  bool finding_center = true;
  hall_low_point = (uint16_t)~0 >> 1;  // set low-point to maximum value (could hard code high value but this is more fun)

 // stepper.setMaxVelocity(800);
 // stepper.setMaxAcceleration(4000);
  hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // get these up front
//  step_low_angle = stepper.encoder.getAngleRaw();
 // stepper.moveAngle(365);


  while (finding_center) {
    // Serial.print("angle moved: ");
    // Serial.println(stepper.angleMoved());
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);
    if (hall_sensor_val <= hall_low_point) {
      hall_low_point = hall_sensor_val;
      Serial.print("New Low Point Recorded: ");
      Serial.print(hall_low_point);
    //  step_low_angle = stepper.encoder.getAngleRaw();
      Serial.print(" angle: ");
      Serial.println(step_low_angle);
    }
 //   if (stepper.angleMoved() >= 360 || stepper.angleMoved() <= -360) {
      //     Serial.println("ending init rotation");
 //     finding_center = false;
 //   }
  }
}


// Use the previously found hall effect sensor low reading to return the mass to the center position
int16_t step_move_home() {
  uint32_t start_time_mS = millis();
  bool home_found = false;
  int16_t hall_sensor_val;
  Serial.println("Moving Home, please stand by..");
  while (!home_found) {
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // measure the hall sensor
    Serial.println(hall_sensor_val);
    // timeout clause here to exit loop if home cannot be found (two functions can be written into their own loop with the output of this function as the escape clause)
    if (millis() - start_time_mS >= (HOMING_TIMEOUT_S * 1000)) {
      Serial.println("step_move_home() timeout");
      /// Serial.println("Moving to Angle 0");
      //stepper.moveToAngle(0);  //
      return 0;
    }
  }


// wraps previous two functions together into a reliable structure
void run_stepper_calibration() {
  bool cal_complete;
  while (!cal_complete) {
    step_find_home();
    cal_complete = step_move_home();
  }
 // stepper.setMaxVelocity(MAX_MOTOR_STEPS_S);
 // stepper.setMaxAcceleration(MAX_MOTOR_ACC_STEPS_S_S);
}


void stepper_setup() {
//  stepper.setup(NORMAL, 400, 10, 0.2, 0.0, 16, true, false, 100, 1);  //Initialize uStepper S32
//  stepper.setCurrent(100);                                            // set motor current as percentage not useable unless current jumper placed in I-PWM position
  //stepper.setHoldCurrent(STEPPER_HOLD_CURRENT);                       // set holding current as percentage
  run_stepper_calibration();
  Serial.print("Current Encoder Pos (raw): ");
 // Serial.println(stepper.encoder.getAngleRaw());

  Serial.print("Current Encoder Pos cooked: ");
 // Serial.println(round(stepper.encoder.getAngle()));
  stepper.stop();
}
