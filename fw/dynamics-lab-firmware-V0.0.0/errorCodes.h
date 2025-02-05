/*
 Template for Error Codes

*/

#pragma once

#include "globals.h"

/*
-2       
-3       Ethernet Cable Likely disconnected                   FATAL
-4       
-5       
-6       
-7      I2C Error                                             FATAL
-8      watchdog timer expired                                WARNING
-9      Temperature Out of Range Detected                     WARNING
-10     Out of Range Value commanded
-11     
-12     
-13     
-14     
-15     
-16     
-17     ESTOP button Pressed                                  FATAL
-18
-19
-20     Unknown JSON command Recieved                         ERROR
-21     Exception in State Machine                            ERROR

*/

// Logging Level Constants
typedef enum {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
} messageLevel;

char debugLevel[5][8] = {
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL"
};

// These variables relate to the JSON report that is printed to serial monitor to report sensor data & all statuses to UI
#define JSON_BUFFER_SIZE 620
StaticJsonDocument<JSON_BUFFER_SIZE> JSONstatus;


int16_t last_warning_code = 0;
uint32_t warning_set_time_mS;

bool FATAL_ERROR = false;
int16_t FATAL_ERROR_CODE = 0;



void print_json_status(bool printPretty = false) {
  if (printPretty) {
    serializeJsonPretty(JSONstatus, Serial);
  } else {
   // serialiseJson(JSONstatus, Serial);
  }
  Serial.print(F("\n"));
}

// returns level of the message or -1 if error in lookup
int get_message_level(const char* message_level) {
  for (int i = 0; i < 5; i++) {
    if (!strcmp(debugLevel[i], message_level)) {
      return i;
    }
  }
  return -1;
}


// Use this to set errors elsewhere, can also be used to reset errors by calling with no arguments
void set_error(bool ok = true, int16_t code = 0, const char* msg = "", messageLevel logLevel = INFO, const char* context = "") {
#if ERROR_LEVEL_PRIORITY_ENABLE == true
  int16_t current_error_level = get_message_level(JSON["level"]);  // Get current error level
  if (logLevel >= current_error_level) {
#endif
    JSONstatus["level"].set(debugLevel[logLevel]);
    JSONstatus["context"].set(context);
    JSONstatus["payload"]["status"]["ok"].set(ok);
    JSONstatus["payload"]["status"]["code"].set(code);
    JSONstatus["payload"]["status"]["msg"].set(msg);
    if (logLevel == FATAL) {
      FATAL_ERROR = true;       //
      FATAL_ERROR_CODE = code;  // log the error that caused the FATA_ERROR as can be cleared later if required
    }
#if DEBUG_ERRORS == true
    Serial.print("DEBUG_ERRORS (set): ");
    Serial.print(code);
    Serial.print("  msg: ");
    Serial.println(msg);
#endif
#if ERROR_LEVEL_PRIORITY_ENABLE == true
  } else {
    // New Error was lower level than previous error so ignore
    // Do nothing
  }
#endif
}


// Same as set error, but always ignores priority
void hard_set_error(bool ok = true, int16_t code = 0, const char* msg = "", messageLevel logLevel = INFO, const char* context = "") {
  JSONstatus["level"].set(debugLevel[logLevel]);
  JSONstatus["context"].set(context);
  JSONstatus["payload"]["status"]["ok"].set(ok);
  JSONstatus["payload"]["status"]["code"].set(code);
  JSONstatus["payload"]["status"]["msg"].set(msg);
  if (logLevel == FATAL) {
    FATAL_ERROR = true;       //
    FATAL_ERROR_CODE = code;  // log the error that caused the FATA_ERROR as can be cleared later if required
  }
}

void clear_error(int16_t error_code, bool clear_fatal = false) {
  int16_t current_error = JSONstatus["payload"]["status"]["code"];
#if DEBUG_ERRORS == true
  Serial.print("DEBUG_ERRORS (clear): current_error: ");
  Serial.print(current_error);
  Serial.print(" clear_code: ");
  Serial.println(error_code);
#endif
  if (current_error == error_code) {
    hard_set_error();  // Set error with no values passed clears the current error
    if (clear_fatal) {
      if (FATAL_ERROR_CODE == error_code) {  // This may lead to confusion if multiple fatal errors ave occured, may need simplifying later
        FATAL_ERROR_CODE = 0;
        FATAL_ERROR = false;
      }
    }
#if DEBUG_ERRORS == true
    Serial.println("Errors Mached & Deleted");
#endif
  }
}




// This function should be called periodically to clear errors with message level "WARNING"
void clear_warning() {
  bool error_status = JSONstatus["payload"]["status"]["ok"];

  if (!error_status) {  // if any error has been triggered (false bool)
                        // Get the error code
    int16_t active_warning_code = JSONstatus["payload"]["status"]["code"];
    // Check the level of the error
    char msg_level[8];
    // TODO A string copy HERE JSON["level"] into msg_level
    strcpy(msg_level, JSONstatus["level"]);

#if DEBUG_ERRORS == true
    Serial.print("Debug Errors: Error Found: ");
    Serial.print(active_warning_code);
    Serial.print(", ");
    Serial.println(msg_level);
#endif

    if (!strcmp(msg_level, "WARNING")) {  // returns 0 if strings match
      if (active_warning_code != last_warning_code) {
        warning_set_time_mS = millis();  // save the time the warning was set
        last_warning_code = active_warning_code;
      } else {  // If active warning is true, start checking time
        if (millis() - warning_set_time_mS >= WARNING_ACTIVE_PERIOD_mS) {
          clear_error(active_warning_code, false);
          last_warning_code = 0;
        }
      }
    }
  }
}
