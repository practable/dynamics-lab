


// Add included external libraries here
//#include <PCA9685.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <stdlib.h>
#include <autoDelay.h>  //https://github.com/PanGalacticTech/autoDelay_library



#define EXPERIMENT_NAME "dynamics-lab"
#define FIRMWARE_VERSION "V0.0.0"
#define DEVELOPER "Imogen-Heard"

// User Options & config
#define DEBUG_STATES false
#define DEBUG_STATE_MACHINE false
#define COMMAND_HINTS false




// Add included internal libraries here
#include "jsonMessenger.h"

// Create objects
jsonMessenger jsonRX;  // create a json messenger object to handle commands received over Serial connection
//File sdFile;           // SD card file object
autoDelay printDelay;  // Delay object for timing functions
//PCA9685 bank_A;        // PWM-controller objects
//PCA9685 bank_B;


#define JSON_SD_DOC_SIZE 2000
StaticJsonDocument<JSON_SD_DOC_SIZE> jsonSDdoc;  // takes the JSON saved on the SD card and recalls it into RAM

#define ARRAY_ROW 2
#define ARRAY_COL 2

//#define COL 5
//#define ROW 5   // TODO make these the same!


const char *pwm_filename = "pwm.txt";
uint16_t brightness = 0;
#define MAX_PWM_VAL 255
#define MIN_OFFSET_VAL -255
//#define MID_POINT 2048

uint16_t global_speed = 0;

#define BANK_A_ADDRESS 0x7F
#define BANK_B_ADDRESS 0x7E

#define PWM_FREQUENCY 1500
                          //{A0, A1, B0, B1}
const int16_t pwm_pins[4] = {9, 3, 6, 5};  // pins chosen for intercompatability with arduino uno
#define NUM_PWM_CHANNELS 4

bool calibration_mode = false;   // in calibration mode, no interpolation is done so values can be edited directly



// Define 5x5 array/matrix for storing PWM vals
// Vertical Axis is A to E
// Hoz Axis 0 to 4
// Dont think these ever get used
#define r_A 0
#define r_B 1
#define r_C 2
#define r_D 3
#define r_E 4

// Working holding array that stores the current offsets
int16_t PWM_array[ARRAY_ROW][ARRAY_COL] = {
  { 0, 0 },
  { 0, 0 }
};

uint16_t lower_bound_speed;
int16_t lower_array[ARRAY_ROW][ARRAY_COL] ={ 
  { 0, 0 },
  { 0, 0 }
};

uint16_t upper_bound_speed;
int16_t upper_array[ARRAY_ROW][ARRAY_COL] ={ 
  { 0, 0 },
  { 0, 0 }
};

int16_t interpolated_array[ARRAY_ROW][ARRAY_COL] = {
  { 0, 0 },
  { 0, 0 }
};



const int16_t blank_matrix[ARRAY_ROW][ARRAY_COL] = {
  { 0, 0 },
  { 0, 0 }
};



char ch_name_array[][2] = {
  "A",
  "B",
  "C",
  "D",
  "E"
};


// Add included internal header files here
#include "sdFunctions.h"
#include "matrixFunctions.h"
#include "pwmController.h"
#include "stateConfig.h"
