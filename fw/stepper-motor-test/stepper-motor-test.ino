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

uint32_t print_delay_mS = 1000 / PRINT_RATE_Hz;

float get_RPM_from_Hz(float Hz) {
  float rpm = Hz * 60.0;
  return rpm;
}



void setup() {
  Serial.begin(115200);
  Serial.println("\nuStepper S32 - Test");
  stepper.setup();                 //Initialize uStepper S32
  stepper.checkOrientation(30.0);  //Check orientation of motor connector with +/- 30 microsteps movement

  //stepper.setRPM(-100);				 //Set speed to -100
  stepper.setCurrent(100);    // set motor current as percentage
  stepper.setHoldCurrent(0);  // set holding current as percentage
  stepper.stop();
}


//setBrakeMode(uint8_t mode, float brakeCurrent = 25.0);


#define COMMAND_SIZE 64
//char command[COMMAND_SIZE];


float rpm = 0;
float Hz = 0;

void loop() {
  char cmd;

  float value;


  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    char command[COMMAND_SIZE];
    Serial.readBytesUntil(10, command, COMMAND_SIZE);

    Serial.print("cmd: ");
    Serial.println(command);

    value = atof(command);

    Serial.print("data: ");
    Serial.println(value);




    if (value == 0) {
      stepper.stop(HARD);
      Serial.println("stop cmd received");
      Hz = 0;
      rpm = 0;
    } else if (value > 0 || value < 0) {
      Hz = value;
      rpm = get_RPM_from_Hz(Hz);
      stepper.setRPM(rpm);
    }
  }

  if (stepper.isStalled()) {
    Serial.println("Motor Has Stalled");
    stepper.setBrakeMode(FREEWHEELBRAKE);
    Hz = 0;
    rpm = 0;
  }

  if (printDelay.millisDelay(print_delay_mS)) {
    float actualRPM = stepper.getDriverRPM();
    float actualHz = actualRPM / 60.0;
    char printBuffer[128];
    char hz_buf[8];
    char rpm_buf[8];
    char a_hz_buf[8];
    char a_rpm_buf[8];

    dtostrf(Hz, 3, 3, hz_buf);
    dtostrf(rpm, 3, 3, rpm_buf);

    dtostrf(actualHz, 3, 3, a_hz_buf);
    dtostrf(actualRPM, 3, 3, a_rpm_buf);

    sprintf(printBuffer, "set_Hz: %4s, Reported_Hz: %4s, Set_RPM: %4s, reported_rpm: %4s, Position, ", hz_buf, a_hz_buf, rpm_buf, a_rpm_buf);
#if PRINT_PERIODIC_UPDATES == true
    Serial.println(printBuffer);
#endif
  }
}
