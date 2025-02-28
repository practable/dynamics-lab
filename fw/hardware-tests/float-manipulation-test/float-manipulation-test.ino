

// Quick method to format float values to be the same width (clean up SerialPrintJSONs)
// This is probably very resource intensive but is only intended to use during testing

struct floatString {
  char* floatVal;
};


void formatFloat(float value, char* buffer, size_t length, int decimalPlaces = 2) {
    if (length < 4) { // Ensure buffer is large enough
        if (length > 0) buffer[0] = '\0';
        return;
    }

    char temp[16]; // Temporary buffer
    dtostrf(value, length - 1, decimalPlaces, temp);

    // Replace '+' with space for positive numbers
    if (temp[0] == '+') temp[0] = ' ';

    // Right-align by shifting if necessary
    size_t tempLen = strlen(temp);
    if (tempLen < length - 1) {
        size_t padding = length - 1 - tempLen;
        memmove(buffer + padding, temp, tempLen + 1);
        memset(buffer, ' ', padding);
    } else {
        strncpy(buffer, temp, length - 1);
        buffer[length - 1] = '\0'; // Ensure null termination
    }
}

// rounds a number to 2 decimal places
// example: round(3.14159) -> 3.14
double round2(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}



float testVal = 1.12345;
float testVal_A = -0.05642;
float testVal_B = -1.05642;
float testVal_C = -100.05642;
float testVal_D = 10.354345;
float testVal_E = 111.1234;




void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test Now:");
  char outputBuffer[10];
  Serial.println(sizeof(outputBuffer));
  formatFloat(testVal, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
  formatFloat(testVal_A, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
  formatFloat(testVal_B, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
  formatFloat(testVal_C, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
  formatFloat(testVal_D, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
  formatFloat(testVal_E, outputBuffer, sizeof(outputBuffer), 3);
  Serial.println(outputBuffer);
}

void loop() {
  // put your main code here, to run repeatedly:
}
