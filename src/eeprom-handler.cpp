#include "eeprom-handler.h"

void setupEEPROM() {
  EEPROM.begin(512);  //Initialize EEPROM
}

int getEEPROMAddress(const char * name) {
  if (strcmp(name, "ssid") == 0) {
    return WIFI_SSID_ADDRESS;
  } else if (strcmp(name, "password") == 0) {
    return WIFI_PASSWORD_ADDRESS;
  } else if (strcmp(name, "magic_string") == 0) {
    return MAGIC_STRING_ADDRESS;
  } else if (strcmp(name, "access_token") == 0) {
    return SENSOR_ACCESS_TOKEN_ADDRESS;
  } else if (strcmp(name, "time_between_measurements") == 0) {
    return TIME_BETWEEN_MEASUREMENTS_ADDRESS;
  } else if (strcmp(name, "max_rtc_records") == 0) {
    return MAX_RTC_RECORDS_ADDRESS;
  }

  return -1;
}

bool readFromEEPROM(char * buf, const char * name) {
  int addr = getEEPROMAddress(name);
  if (addr < 0) {
    ardprintf("EEPROM: Could not read unknown value %s", name);
    return false;
  }

  for(int i=0; i < 60; i++) {
    char ch = (char)EEPROM.read(addr+i);
    buf[i] = ch;

    if (ch == '\0') {
      break;
    }
  }

  ardprintf("EEPROM: read %s: %s", name, buf);
  return true;
}

uint8_t readIntFromEEPROM(const char * name) {
  int addr = getEEPROMAddress(name);
  if (addr < 0) {
    ardprintf("EEPROM: Could not read unknown value %s", name);
    return false;
  }

  char ch = (uint8_t)EEPROM.read(addr);

  ardprintf("EEPROM: read %s: %d", name, ch);
  return ch;
}

bool isConfigSaved() {
  #ifdef PRECONFIGURED
    return true;
  #endif
  
  char magicString[60];
  readFromEEPROM(magicString, "magic_string");

  if (strcmp(magicString, xstr(MAGIC_STRING)) != 0) {
    ardprintf("EEPROM: No WiFi credentials saved");
    return false;
  }

  return true;
}

bool clearConfig() {
  ardprintf("EEPROM: Clearing Wi-Fi credentials");
  return saveToEEPROM("magic_string", "");
}

bool saveConfig(
  const char * ssid, 
  const char * password, 
  const char * sensorAccessToken,
  uint8_t timeBetweenMeasurements, // min
  uint8_t maxRtcRecords // how many records before they are sent to server
) {
  if (!ssid || strlen(ssid) == 0 || !password || strlen(password) == 0) {
    ardprintf("Need SSID & password to save config");
    return false;
  }
  
  bool retVal = true;

  if (
    !saveToEEPROM("ssid", ssid) ||
    !saveToEEPROM("password", password) ||
    !saveToEEPROM("access_token", sensorAccessToken) ||
    !saveIntToEEPROM("time_between_measurements", timeBetweenMeasurements) ||
    !saveIntToEEPROM("max_rtc_records", maxRtcRecords) ||
    !saveToEEPROM("magic_string", xstr(MAGIC_STRING))
  ) {
    ardprintf("Could not save some config data to EEPROM");
    retVal = false;
  }

  return retVal;
}

bool saveToEEPROM(const char * name, const char * value) { 
  int addr = getEEPROMAddress(name);

  if (addr < 0) {
    ardprintf("EEPROM: Unknown address for %s", name);
    return false;
  }
 
  for(int i=0; i < (int)strlen(value); i++) {
    EEPROM.write(addr + i, value[i]);
  }
  EEPROM.write(addr + strlen(value), '\0');

  bool success = EEPROM.commit(); 

  if (!success) {
    ardprintf("EEPROM: Could not save");
    return false;
  }

  ardprintf("EEPROM: Saved variable %s with value %s", name, value);
  return true;
}

bool saveIntToEEPROM(const char * name, uint8_t value) { 
  int addr = getEEPROMAddress(name);

  if (addr < 0) {
    ardprintf("EEPROM: Unknown address for %s", name);
    return false;
  }
 
  EEPROM.write(addr, (char)value);

  bool success = EEPROM.commit(); 

  if (!success) {
    ardprintf("EEPROM: Could not save");
    return false;
  }

  ardprintf("EEPROM: Saved variable %s with value %d", name, value);
  return true;
}