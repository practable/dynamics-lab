
//#include <Servo.h>
#include <NewServo.h> // Available @ https://github.com/GhassanYusuf/NewServo
#include <autoDelay.h>

autoDelay servoDelay;

//Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

NewServo servo(6);

int pos = 0;  // variable to store the servo position

#define COMMAND_SIZE 64

#define SERVO_PPM_PIN 6

#define SERVO_ZERO_POS 0
#define SERVO_OPEN_POS 20

void setup() {
  servo.begin();
  servo.setInit(-90);
  servo.setMin(-90);
  servo.setMax(20);

  // with current settings passing 0 and -90 causes it to go full distance

  /// servo.attach(SERVO_PPM_PIN);
  // servo.write(0);
  delay(1000);
}

int16_t hall_sensor_value;

int angle;

int servo_target = 0;

int microseconds;



void loop() {

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
    servo_target = int(value);
    // servo.write(servo_target);
    Serial.print("target: ");
    Serial.println(servo_target);
    servo.move(servo_target);
  }
  // angle = servo.read();
  // microseconds = servo.readMicroseconds();
  // Serial.print("angle: ");
  // Serial.print(angle);
  // Serial.print(" mS: ");
  // Serial.println(microseconds);
}
