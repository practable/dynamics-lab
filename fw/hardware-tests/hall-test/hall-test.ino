
#define HALL_SENSOR_PIN A4

/*

Hall effect sensor pin to 12k resistor
MCU GPIO to 12k & 22k resistor
22k resistor to GND

*/



void setup() {
}

int16_t hall_sensor_value;

void loop() {

  hall_sensor_value = analogRead(HALL_SENSOR_PIN);
  Serial.println(hall_sensor_value);
  delay(200);
}
