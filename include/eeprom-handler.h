#ifndef EEPROM_HANDLER_H_INCLUDED
#define EEPROM_HANDLER_H_INCLUDED

#include <Arduino.h>
#include <EEPROM.h>
#include "common.h"

#define MAGIC_STRING_ADDRESS 0
#define STATE_MACHINE_ADDRESS 32
#define WIFI_SSID_ADDRESS 64
#define WIFI_PASSWORD_ADDRESS 96
#define SENSOR_ACCESS_TOKEN_ADDRESS 128 // length 37 chars + null terminator
#define TIME_BETWEEN_MEASUREMENTS_ADDRESS 166
#define MAX_RTC_RECORDS_ADDRESS 167

#define MAGIC_STRING "12345678"

#define CFG_WIFI_SSID xstr(WIFI_SSID)
#define CFG_WIFI_PASSWORD xstr(WIFI_PASSWORD)

using namespace std;

void setupEEPROM();
bool saveConfig(
    const char * ssid, 
    const char * password, 
    const char * sensorAccessToken,  
    uint8_t timeBetweenMeasurements = 15, //min
    uint8_t maxRtcRecords = 4
);
bool saveToEEPROM(const char * name, const char * value);
bool saveIntToEEPROM(const char * name, uint8_t value);
bool isConfigSaved();
bool clearConfig();
bool readFromEEPROM(char * buf, const char * name);
uint8_t readIntFromEEPROM(const char * name);

#endif