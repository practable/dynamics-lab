/* stepperFunctions.h

 Any functions that relate to the operation of the uStepper32 stepper motor

*/

#pragma once

float get_RPM_from_Hz(float Hz) {
  float rpm = Hz * 60.0;
  return rpm;
}

float get_Hz_from_RPM(float rpm) {
  float Hz = rpm / 60.0;
  return Hz;
}

void home_position(){
  
}

// do one full revolution to find the lowest value reported by the hall effect sensor
void step_find_home() {
  Serial.println(F("{\"mode\":\"finding-home-position\"}"));

  int16_t hall_sensor_val;
  bool finding_center = true;
  hall_low_point = (uint16_t)~0 >> 1;  // set low-point to maximum value (could hard code high value but this is more fun)

  stepper.setMaxVelocity(800);
  stepper.setMaxAcceleration(4000);
  hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // get these up front
  step_low_angle = stepper.encoder.getAngleRaw();
  stepper.moveAngle(365);


  while (finding_center) {
    // Serial.print("angle moved: ");
    // Serial.println(stepper.angleMoved());
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);
    if (hall_sensor_val <= hall_low_point) {
      hall_low_point = hall_sensor_val;
      //Serial.print("New Low Point Recorded: ");
      // Serial.print(hall_low_point);
      step_low_angle = stepper.encoder.getAngleRaw();
      // Serial.print(" angle: ");
      //Serial.println(step_low_angle);
    }
    if (stepper.angleMoved() >= 360 || stepper.angleMoved() <= -360) {
      Serial.println("ending init rotation");
      finding_center = false;
    }
  }
}


// Use the previously found hall effect sensor low reading to return the mass to the center position
int16_t step_move_home() {
  Serial.println(F("{\"mode\":\"moving-home\"}"));
  uint32_t start_time_mS = millis();
  uint32_t resetable_start = start_time_mS;
  int16_t wait_time_one = 2000;
  bool home_found = false;
  int16_t hall_sensor_val;
  while (!home_found) {
    hall_sensor_val = analogRead(HALL_SENSOR_PIN);  // measure the hall sensor
    //Serial.println(hall_sensor_val);

    // Function to start reducing the sensitivity of the homing algorithm after some time
    if (millis() - resetable_start >= wait_time_one) {
      hall_low_point++;
      wait_time_one = wait_time_one - 200;  // speed up as time progresses
      resetable_start = millis();
    }

    // timeout clause here to exit loop if home cannot be found (two functions can be written into their own loop with the output of this function as the escape clause)
    if (millis() - start_time_mS >= (HOMING_TIMEOUT_S * 1000)) {
      Serial.println(F("\"mode\":\"homing-function-timeout\""));
      Serial.println(F("\"warning\":\"motor position is uncalibrated\""));
      stepper.moveToAngle(0);  //
      return 0;
    }
    if (hall_sensor_val == hall_low_point) {  // when the low point is found again, run homing sequence
      stepper.stop();
      home_found = true;
      Serial.print(F("{\"mode\":\"home-found\",\"raw-angle\":\""));
      Serial.print(stepper.encoder.getAngleRaw());
      Serial.println(F("\"}"));
      stepper.moveAngle(160);  // this method seems to be under performing by factor of 10!
      //stepper.moveAngle(180);
      while (stepper.getMotorState(POSITION_REACHED)) {
        ;  // wait for motor to reach position
      }
      stepper.encoder.setHome();
      Serial.print(F("{\"mode\":\"resetting-home\",\"raw-angle\":\""));
      Serial.print(stepper.encoder.getAngleRaw());
      Serial.println(F("\"}"));
      stepper.stop();
      stepper.setBrakeMode(COOLBRAKE);
      delay(100);
    } else {
      stepper.moveSteps(10);
      stepper.setRPM(60);
    }
  }
  return 1;  // on success return 1
}



// wraps previous two functions together into a reliable structure
void run_stepper_calibration() {
  bool cal_complete;
  while (!cal_complete) {
    step_find_home();
    cal_complete = step_move_home();
  }
  stepper.setMaxVelocity(MAX_MOTOR_STEPS_S);
  stepper.setMaxAcceleration(MAX_MOTOR_ACC_STEPS_S_S);
}


void stepper_setup(bool run_calibration = true) {
  stepper.setup(NORMAL, STEPPER_STEPS, 10, 0.2, 0.0, 16, true, false, 100, 1);  //Initialize uStepper S32
  stepper.setCurrent(100);                                            // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(STEPPER_HOLD_CURRENT);                       // set holding current as percentage
  if (run_calibration) {
    run_stepper_calibration();
    Serial.print(F("{\"mode\":\"init-encoder-pos\",\"raw\":\""));
    Serial.print(stepper.encoder.getAngleRaw());
    Serial.print(F("\",\"cooked\":\""));
    Serial.print(round(stepper.encoder.getAngle()));
    Serial.println(F("\"}"));
  }
  stepper.stop();
}
