
#include <Servo.h>
#include <autoDelay.h>

autoDelay servoDelay;

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;  // variable to store the servo position

#define SERVO_PPM_PIN 6

#define SERVO_ZERO_POS 0
#define SERVO_OPEN_POS 20

void setup() {
  servo.attach(SERVO_PPM_PIN);
  servo.write(0);
  delay(1000);
}

int16_t hall_sensor_value;

int angle_addr = 5;

int servo_target = 0;

int microseconds;

void loop() {

  servo.write(servo_target);
  microseconds = servo.read();
  Serial.println(microseconds);

  if (servoDelay.secondsDelay(5)) {
    if (servo_target == SERVO_OPEN_POS) {
      servo_target = SERVO_ZERO_POS;

    } else if (servo_target == SERVO_ZERO_POS) {
      servo_target = SERVO_OPEN_POS;

    }
  }
}
