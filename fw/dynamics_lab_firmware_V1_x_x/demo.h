/* demo.h

 State machine to run entirely within some other state, and show off some of the dynamic features of the experiment hardware

 Imogen 15/07/2025

 - Ensure that all functions are non-blocking

*/
#include "globals.h"



#define DEMO_SMALL_TIMER_mS 750
#define DEMO_MEDIUM_TIMER_mS 1500
#define DEMO_LONG_TIMER_mS 20000  // changes states

uint8_t demoState = 1;
uint32_t last_long_time_mS = 0;
uint32_t last_short_time_mS = 0;
uint32_t last_mid_time_mS = 0;
float motor_hz = 0;  // keep all demo items completely seperate so nothing to intefere with working program
float motor_hz_modifyer = 0.25;
int16_t flash_modifyer = 75;
int16_t led_on_time = 1000;
int16_t led_off_time = 1000;

// function to end demo quickly and return all hardware back to useable state (should be placed after receiving any message)
void end_demo() {
  motorState = STOPPED;
  stepper.stop(HARD);
  stepper.setMaxVelocity(MAX_MOTOR_STEPS_S);  // If these have been set higher elsewhere, make sure they are set back to normal here
  stepper.setMaxAcceleration(MAX_MOTOR_ACC_STEPS_S_S);
  beacon.stopBlink();
  demoState = -1;
}

void start_demo() {
  stepper.setMaxVelocity(800);
  stepper.setMaxAcceleration(4000);
  last_long_time_mS = millis();
  flash_modifyer = 75;
  demoState = 2;
}

// Spin the blade up to 8 Hz and Down with a slow ramp, ramping the LED flashing at the same time
void demo_state_one() {
  motorState = MOMENTARY;
  int16_t randomTime = random(-1000, 1000);
  if (millis() - last_mid_time_mS >= (DEMO_MEDIUM_TIMER_mS + randomTime)) {
    last_mid_time_mS = millis();
    motor_hz += motor_hz_modifyer;
    //flash_modifyer += 50;
    led_on_time -= flash_modifyer / 3;
    led_off_time -= flash_modifyer;
    if (led_on_time <= 0) {
      led_on_time = 400;
    }
    if (led_off_time <= 0) {
      led_off_time = 400;
    }
    beacon.startBlink(led_on_time, led_off_time);
    int randomNum = random(-360, 360);
    stepper.moveAngle(randomNum);
    Serial.print("ontime: ");
    Serial.print(led_on_time);
    Serial.print(" offtime: ");
    Serial.print(led_off_time);
    Serial.print(" randTime: ");
    Serial.print(randomTime);
    Serial.print(" rangle: ");
    Serial.println(randomNum);
  }
  if (millis() - last_long_time_mS >= DEMO_LONG_TIMER_mS) {
    demoState = 3;
  }
}

void demo_state_two() {

  demoState = 0;
}

//void functionArray[2] = { demo_state_zero, demo_state_zero };

int16_t demoSM() {
  switch (demoState) {
    case 0:
      end_demo();
      return -1;
      break;
    case 1:
      start_demo();
      break;
    case 2:
      demo_state_one();
      break;
    case 3:
      demo_state_two();
      break;
    default:
      end_demo();
      return -1;
      break;
  }
  return demoState;
}