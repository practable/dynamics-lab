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



void home_position() {
}





/* Stall Detection
_Problem is its very hard to detect actual stalls and seperate them from "false stalls"_

Option A:
Stall check should register a stall event then start a counter, if stall events are still happening after time X then
stall is returned true.
 - Couldnt work out finer points of implementation

Option B:
Stall hits are accrued every time it detects non movement, but this scale decreases as a function of time, rather than every time a non-stall is detected
  - Working out how fast to decay stall hits may require lots of tuning and be fragile

Option C:
 - Stop trying to compare the encoder value to itself, and instead compare the set velocity from the actual velocity
*/



// Option B
bool check_for_stall() {
  uint16_t new_encoder_value = stepper.encoder.getAngleRaw();
  int16_t diff = new_encoder_value - last_encoder_value;
  last_encoder_value = new_encoder_value;
  if (diff > 3 || diff < -3) {                                    //// Stall condition unlikely
    if (stalls_detected > 0) {                                    // prevent stall_detected var from rolling over negative
      if (millis() - last_stall_time_mS >= STALL_HIT_DECAY_mS) {  /// lets make this a timer instead
        stalls_detected--;
      }
    }
    return false;
  } else {                                  //Likely stall condition detected
    last_stall_time_mS = millis();          // get the time of this stall
                                            //    if (stalls_detected == 0) {                    // if first time stall detected
                                            //      stall_trigger_time_mS = last_stall_time_mS;  // record time THIS DOES NOTHING
                                            //    }
    stalls_detected = stalls_detected + 1;  // make it accumilate faster than deaccumilate?
                                            //    Serial.print("stall limit: ");
                                            //    Serial.print(stall_limit);
                                            //    Serial.print(" Detected: ");
                                            //    Serial.println(stalls_detected);
    if (stalls_detected > stall_limit) {
      stalls_detected = 0;
      Serial.println(F("{\"WARNING\":\"Motor may be stalled, stopping\"}"));
      return true;
    } else {
      return false;
    }
  }
}


void stepper_setup() {
  stepper.setup(NORMAL, STEPPER_STEPS, 10, 0.2, 0.0, 16, true, false, 100, 1);  //Initialize uStepper S32
  stepper.setCurrent(100);                                                      // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(STEPPER_HOLD_CURRENT);                                 // set holding current as percentage
  stepper.setMaxVelocity(MAX_MOTOR_STEPS_S, false);
  stepper.setMaxAcceleration(MAX_MOTOR_ACCELLERATION);
  stepper.stop();
}

