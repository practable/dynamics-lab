#include "errorRep.h"



// Constructor
errorRep::errorRep() {
}



void errorRep::print_json_status(bool printPretty) {
  if (printPretty) {
    serializeJsonPretty(JSONstatus, Serial);
  } else {
    serializeJson(JSONstatus, Serial);
  }
  Serial.print(F("\n"));
}




// returns level of the message or -1 if error in lookup
int errorRep::get_message_level(const char* message_level) {
  for (int i = 0; i < 5; i++) {
    if (!strcmp(debugLevel[i], message_level)) {
      return i;
    }
  }
  return -1;
}




// Use this to set errors elsewhere, can also be used to reset errors by calling with no arguments
void errorRep::set_error(bool ok, int16_t code, const char* msg, messageLevel logLevel, const char* context) {
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
void errorRep::hard_set_error(bool ok, int16_t code, const char* msg, messageLevel logLevel, const char* context) {
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




void errorRep::clear_error(int16_t error_code, bool clear_fatal) {
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
void errorRep::clear_warning() {
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

    if (!strcmp(msg_level, debugLevel[WARNING])) {  // returns 0 if strings match
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