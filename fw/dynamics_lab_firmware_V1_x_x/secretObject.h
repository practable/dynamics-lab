/* Class for managing storing and retreving calibration data from persistant memory


method developed by: David Reid
Author: Imogen Wren
14/07/2025


*/


//#pragma once

#ifndef secretObject_h
#define secretObject_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <FlashStorage_STM32.h>



#define SECRETOBJECT_VERSION "V0.0.1"
#define START_ADDRESS 0xFF  // define start point for any persistant memory use
#define MAX_WRITES 20       // define number of writes untill firmware must be re-progammedtk

#define MEMORY_OBJECT FlashStorage_STM32 // might actually just be EEPROM  // replace with suitable persistant memory object for platform
// Arduino SAMD21: FlashStorage    // untested
// Arduino ARM: EEPROM


class secretObject {
public:

  // Create a structure that stores the cal data
  typedef struct {
    boolean secure;               // Set this true when secret is first written
    boolean valid;                // Set this true when calibration data is first written
    char secret[SECRET_LEN_MAX];  // Secret string for authorising calibration updates (typically a uuid of 36 chars in form 8-4-4-4-12)
    int writes;                   // Count number of remaining writes we'll permit
    // You can change the values below here to suit your experiment
    int16_t calData;  // Scale factors / calibration / offset data
  } Calibration;

  // Create a global "Calibration" variable and call it cal
  Calibration cal;


  // Reserve a portion of flash memory to store a "Calibration" and
  // call it "cal_store".
  MEMORY_OBJECT(cal_store, Calibration);




private:
};



#endif
