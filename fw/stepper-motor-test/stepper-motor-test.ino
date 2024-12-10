/********************************************************************************************
* 	    	File:  continous.ino                                                              *
*		    Version:    2.3.0                                          						    *
*      	Date: 		October 7th, 2023 	                                    			*
*       Author:  Thomas Hørring Olsen                                                       *
*  Description:  Continous Example Sketch!                                                  *
*                This example demonstrates how the library can be used to make the motorrun *
*                continously, in both directions and making it stop an ongoing movement.    *
*                                                                                           *
* For more information, check out the documentation:                                        *
*    http://ustepper.com/docs/usteppers/html/index.html                                     *
*                                                                                           *
*                                                                                           *
*********************************************************************************************
*	(C) 2023                                                                                  *
*                                                                                           *
*	uStepper ApS                                                                              *
*	www.ustepper.com                                                                          *
*	administration@ustepper.com                                                               *
*                                                                                           *
*	The code contained in this file is released under the following open source license:      *
*                                                                                           *
*			Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International               *
*                                                                                           *
* 	The code in this file is provided without warranty of any kind - use at own risk!       *
* 	neither uStepper ApS nor the author, can be held responsible for any damage             *
* 	caused by the use of the code contained in this file !                                  *
*                                                                                           *
*                                                                                           *
********************************************************************************************/

/*
*      Continous Example Sketch!
*
* This example demonstrates how the library can be used to make the motorrun continously,
* in both directions and making it stop an ongoing movement.
* For more information, check out the documentation:
* http://ustepper.com/docs/usteppers/html/index.html
*/
#include <UstepperS32.h>
UstepperS32 stepper;
#include "TinyMPU6050.h"
MPU6050 mpu;
#include <autoDelay.h>
autoDelay printDelay;

#define PRINT_RATE_Hz 2
#define PRINT_PERIODIC_UPDATES true

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

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nuStepper S32 - Test");


  mpu.Initialize();  // Initialization of MPU
  mpu.Calibrate();   // Calibration - LED will blink until calibration is done !
  delay(3000);
  stepper.setup(NORMAL, 400, 10, 0.2, 0.0, 16, false, 0, 50, 30);  //Initialize uStepper S32
  //stepper.setCurrent(100);     // set motor current as percentage not useable unless current jumper placed in I-PWM position
  stepper.setHoldCurrent(1);  // set holding current as percentage


  // Here need  to figure out the current encoder position

  // Serial.print("Current Encoder Pos Raw: ");
  // Serial.println(stepper.encoder.getAngleRaw());


  //  Serial.print("Current Encoder Pos : ");
  //  Serial.println(stepper.encoder.getAngle());

  //  delay(5000);


  //  stepper.setBrakeMode(FREEWHEELBRAKE);
  stepper.setRPM(200);

  delay(2000);
  //while (!calibrated) {
  for (int i = 0; i < 10000; i++) {
    mpu.Execute();
    float accX = mpu.GetAccX();
    Serial.print("accX: ");
    Serial.println(accX);
    if (accX > cal_plus_acc) {
      cal_plus_acc = accX;
      cal_plus_angle = CONVERTENCODERRAWTOANGLE(stepper.encoder.getAngleRaw());
    }
    if (accX < cal_minus_angle) {
      cal_minus_acc = accX;
      cal_minus_angle = CONVERTENCODERRAWTOANGLE(stepper.encoder.getAngleRaw());
    }
    // Serial.print(CONVERTENCODERRAWTOANGLE(stepper.encoder.getAngleRaw()));
    //  Serial.print(" <- Raw  cooked -> ");
    //  Serial.println(CONVERTENCODERRAWTOANGLE(stepper.encoder.getAngle()));
  }
  stepper.stop();
  stepper.setBrakeMode(FREEWHEELBRAKE);
  delay(3000);
  Serial.print("Cal Plus Angle: ");
  Serial.println(cal_plus_angle);

  Serial.print("Cal Minus Angle: ");
  Serial.println(cal_minus_angle);

  delay(2000);
  Serial.println("Setting Home: ");
  // stepper.encoder.setHome(cal_plus_angle);
  Serial.println("Moving Home: ");
  bool home = false;
  float angle;

  while (!home) {
    angle = CONVERTENCODERRAWTOANGLE(stepper.encoder.getAngleRaw());
    Serial.print(angle);
    Serial.print(" ~= ");
    Serial.println(cal_plus_angle + 50.0);
    if (angle > ((cal_plus_angle + 50.0) - 0.3) && angle < ((cal_plus_angle + 50.0) + 0.3)) {
      home = true;
      Serial.println("found home");
      stepper.encoder.setHome();
    }
    stepper.moveSteps(3);
  }



  // stepper.moveToAngle(-10.0);
  delay(200);



  //  Serial.print("Magnet Detected, for realseasis?  ");
  //  Serial.println(stepper.encoder.detectMagnet());

  Serial.print("Current Encoder Pos (raw): ");
  Serial.println(stepper.encoder.getAngleRaw());

  Serial.print("Current Encoder Pos cooked: ");
  Serial.println(stepper.encoder.getAngle());


  // stepper.encoder.setHome();

  //while(stepper.encoder.getAngle() != 0){

  //}





  // stepper.checkOrientation(30.0);  //Check orientation of motor connector with +/- 30 microsteps movement
  //stepper.setRPM(-100);				 //Set speed to -100

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
