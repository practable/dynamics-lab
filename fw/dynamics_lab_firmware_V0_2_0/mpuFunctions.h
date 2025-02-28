

#pragma once



// For calibration
#define DISCARDED_MEASURES 100
#define CALIBRATION_MEASURES 2000
#define CHECKING_MEASURES 50
#define ACCEL_PREOFFSET_MAGIC_NUMBER 8
#define GYRO_PREOFFSET_MAGIC_NUMBER 4


void mpu_calibration() {  // because adafruit are too lazy to implement this within their own library
  for (int i = 0; i < DISCARDED_MEASURES; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    delay(2);
  }

  float sumAccX = 0;
  float sumAccY = 0;
  float sumAccZ = 0;

  float sumGyroX = 0;
  float sumGyroY = 0;
  float sumGyroZ = 0;
  int cnt = 0;
  for (int i = 0; i < CALIBRATION_MEASURES; i++) {
    cnt++;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    sumAccX += a.acceleration.x / G_CONST;
    sumAccY += a.acceleration.y / G_CONST;
    sumAccZ += a.acceleration.z / G_CONST;
    sumGyroX += g.gyro.x;
    sumGyroY += g.gyro.y;
    sumGyroZ += g.gyro.z;
    delay(2);
    if (cnt % 30 == 0)
      digitalWrite(PC4, !digitalRead(PC4));  // ??
  }
  pinMode(PC4, INPUT);  // ??

  sumAccX /= CALIBRATION_MEASURES;
  sumAccY /= CALIBRATION_MEASURES;
  sumAccZ /= CALIBRATION_MEASURES;

  sumGyroX /= CALIBRATION_MEASURES;
  sumGyroY /= CALIBRATION_MEASURES;
  sumGyroZ /= CALIBRATION_MEASURES;

  acc_offset.X = sumAccX;
  acc_offset.Y = sumAccY;
  acc_offset.Z = sumAccZ;

  gyro_offset.X = sumGyroX;
  gyro_offset.Y = sumGyroY;
  gyro_offset.Z = sumGyroZ;
  // debugging
 // Serial.print("Z acc offset: ");   // no longer applying this offset because it didnt work
 // Serial.println(acc_offset.Z);
 // delay(2000);
}



void mpu_setup() {
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  mpu_calibration();
}