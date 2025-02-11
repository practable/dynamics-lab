


// Function not quite working as expected
// Try to guage home position by wobbling motor and looking for the point of greatest deflection
void wobble_home(){
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
}