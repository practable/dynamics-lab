/*  jsonReporter.h

 Functions to load data into a json template and print to Serial Output

*/




#include "globals.h"



void update_json(int16_t num_samples) {
  StaticJsonDocument<JSON_TX_BUFFER_SIZE> jsonTX;
  //freeRAM = ram.getPrintStats("update_json");
  // Header
  jsonTX[F("timestamp")].set(millis());  //Message Timestamp set this last before printing
                                         // Payload
  jsonTX[F("payload")][F("state")].set(stateNames[smState]);
  jsonTX[F("payload")][F("samples")].set(num_samples);
  // Output Data
  // Stepper Motor Settings
  jsonTX[F("payload")][F("step")][F("hz")].set(step_hz);
  jsonTX[F("payload")][F("step")][F("rpm")].set(step_rpm);
  // Encoder Data

  jsonTX[F("payload")][F("encode")][F("hz")].set(get_Hz_from_RPM(stepper.encoder.getRPM()));
  jsonTX[F("payload")][F("encode")][F("rpm")].set(stepper.encoder.getRPM());
  jsonTX[F("payload")][F("encode")][F("pos(raw)")].set(stepper.encoder.getAngleRaw());
  //  jsonTX[F("payload")][F("encode")][F("pos")].set(stepper.encoder.getAngle());

  // MPU Data
  // For single datapoints
  //  jsonTX[F("payload")][F("mpu")][F("acc")][F("x")].set(mpu.GetAccX());
  //  jsonTX[F("payload")][F("mpu")][F("acc")][F("y")].set(mpu.GetAccY());
  //  jsonTX[F("payload")][F("mpu")][F("acc")][F("z")].set(mpu.GetAccZ());
  //  jsonTX[F("payload")][F("mpu")][F("gyro")][F("x")].set(mpu.GetGyroX());
  //  jsonTX[F("payload")][F("mpu")][F("gyro")][F("y")].set(mpu.GetGyroY());
  //  jsonTX[F("payload")][F("mpu")][F("gyro")][F("z")].set(mpu.GetGyroZ());

  // For Arrays
  // create all the JSONarrays
  JsonArray posArray = jsonTX[F("payload")][F("encode")].createNestedArray("pos");     ##TODO MAKE THESE ARRAYS LESSS CHARACTERS AFTER . POINT
  #TODO MAKE SURE ARRAYS HAVE CORRECT NUMBER OF DATAPOINTS
  JsonArray accXarray = jsonTX[F("payload")][F("mpu")][F("acc")].createNestedArray("x");
  JsonArray accYarray = jsonTX[F("payload")][F("mpu")][F("acc")].createNestedArray("y");
  JsonArray accZarray = jsonTX[F("payload")][F("mpu")][F("acc")].createNestedArray("z");
  JsonArray gyroXarray = jsonTX[F("payload")][F("mpu")][F("gyro")].createNestedArray("x");
  JsonArray gyroYarray = jsonTX[F("payload")][F("mpu")][F("gyro")].createNestedArray("y");
  JsonArray gyroZarray = jsonTX[F("payload")][F("mpu")][F("gyro")].createNestedArray("z");

  // for loop to place data into arrays (is it possible to just pass whole string without looping up to 200 times!?)

  for (int i = 0; i < num_samples; i++) {
    // Serial.print("i: ");
    // Serial.print(i);
    // Serial.print(", ");
    posArray.add(encode_array[i]);
    accXarray.add(accX_array[i]);
    accYarray.add(accX_array[i]);
    accZarray.add(accX_array[i]);
    gyroXarray.add(gyroX_array[i]);
    gyroYarray.add(gyroX_array[i]);
    gyroZarray.add(gyroX_array[i]);
    //Serial.print(encode_array[i]);
    //  Serial.print(", ");
  }
  // Serial.println();



  // Meta Data & Debugging
  //jsonTX[F("meta")][F("sizeof")].set(sizeof(jsonTX));


  // int JSON_bytes = sizeof(jsonTX);  // used for debugging
  //Serial.print("Size of JSON: ");
  //Serial.println(JSON_bytes);

  if (PRINT_JSON) {
#if PRETTY_PRINT_JSON == true
#pragma JSON set to Pretty Print !-May experience problems with Machine Parsing
    serializeJsonPretty(jsonTX, Serial);
#else
#pragma JSON set to standard JSON output - For human readability try enabling Pretty Print !
    serializeJson(jsonTX, Serial);
#endif
    Serial.print(F("\n"));
  }
}