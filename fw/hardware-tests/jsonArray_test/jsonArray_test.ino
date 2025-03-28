#include <ArduinoJson.h>  // installed version 6.21.5 [Arduino Library Manager]

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\nJus' Testin'");

  StaticJsonDocument<256> doc;


  doc["payload"]["test"].set(55);
  
  JsonArray array = doc["payload"]["ar"].createNestedArray("vals");

  array.add(20);
  array.add(30);
  array.add(40);
  array.add(50);
  array.add(60);
  array.add(70);

  serializeJsonPretty(doc, Serial);
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
}
