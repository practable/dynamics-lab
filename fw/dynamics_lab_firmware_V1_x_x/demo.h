/* demo.h

 State machine to run entirely within some other state, and show off some of the dynamic features of the experiment hardware

 Imogen 15/07/2025

 - Ensure that all functions are non-blocking

*/
#include "globals.h"

uint8_t demoState = 0;

#define DEMO_SMALL_TIMER_mS 100
#define DEMO_MEDIUM_TIMER_mS 2000
#define DEMO_LONG_TIMER_mS 20000

uint32_t last_long_time_mS = 0;
uint32_t last_short_time_mS = 0;
uint32_t last_mid_time_mS = 0;
float motor_hz = 0;  // keep all demo items completely seperate so nothing to intefere with working program
float motor_hz_modifyer = 0.1;

// Spin the blade up to 8 Hz and Down with a slow ramp, ramping the LED flashing at the same time
void demo_state_zero() {
  if (millis() - last_short_time_mS >= DEMO_SMALL_TIME_mS) {
    motor_hz += motor_hz_modifyer;
  }
  if (motor_hz > 8) {
    motor_hz_modifyer = -0.2;
  }
  if (motor_hz_modifyer < 0) {
    demoState = 1;
  }
}

void demo_state_one() {

  demoState = -1;
}

void functionArray[2] = { demo_state_zero, demo_state_zero };

int16_t demoSM() {
  functionArray[demoState]();
  return demoState;
}