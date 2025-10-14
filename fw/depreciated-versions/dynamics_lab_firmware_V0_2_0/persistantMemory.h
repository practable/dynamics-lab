#pragma once

#include "globals.h"


// Gets value from emulated EEProm and updates global val
// -> Returns value
int16_t get_offset_from_memory() {
  int signature;
  EEPROM.get(EEPROM_ENCODE_OFFSET_ADDRESS, signature);
  if (signature != WRITTEN_SIGNATURE) {
    Serial.println(F("{\"warning\":\"No offset found in EEPROM\",\"setting_to\":\"0\"}"));
    persistant_encoder_offset = 0;
  } else {
    EEPROM.get(EEPROM_ENCODE_OFFSET_ADDRESS, signature);
    persistant_encoder_offset = EEPROM.read(EEPROM_ENCODE_OFFSET_ADDRESS + sizeof(WRITTEN_SIGNATURE));
    Serial.print("{\"EEPROM_SIGNATURE\":\"0x");
    Serial.print(signature, HEX);
    Serial.print("\",\"encoder_offset\":\"");
    Serial.print(persistant_encoder_offset);
    Serial.println("\"}");
  }
  return persistant_encoder_offset;
}

// Warning: uses finite write cycles, do not call often
// Returns -1 for error
int16_t put_offset_into_memory(int16_t new_offset) {
  int signature;
  EEPROM.get(EEPROM_ENCODE_OFFSET_ADDRESS, signature);
  if (signature != WRITTEN_SIGNATURE) {
    Serial.print(F("{\"info\":\"No signature in EEPROM\",\"writing\":\"0x"));
    Serial.print(WRITTEN_SIGNATURE, HEX);
    Serial.print("\",\"value\":");
    Serial.print(new_offset);
    Serial.println("\"}");
    EEPROM.put(EEPROM_ENCODE_OFFSET_ADDRESS, WRITTEN_SIGNATURE);
  } else {
    Serial.print(F("{\"info\":\"found\",\"signature\":\"0x"));
    Serial.print(signature, HEX);
    Serial.print("\",\"writing\":\"");
    Serial.print(new_offset);
    Serial.println("\"}");
    EEPROM.put(EEPROM_ENCODE_OFFSET_ADDRESS, WRITTEN_SIGNATURE);
    persistant_encoder_offset = new_offset;  // make sure global is updated with new value written
  }
  EEPROM.put(EEPROM_ENCODE_OFFSET_ADDRESS + sizeof(WRITTEN_SIGNATURE), new_offset);
  persistant_encoder_offset = new_offset;  // make sure global is updated with new value written
  return 0;
}