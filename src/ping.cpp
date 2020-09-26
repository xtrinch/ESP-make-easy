#include "ping.h"

unsigned long lastPingMillis = 0;

bool sendPing() {
  char url[150];
  snprintf(url, 500, "http://%s/api/forwarders/ping", CFG_SENSOR_DASHBOARD_URL);
  return makeNetworkRequest(url, CFG_ACCESS_TOKEN, "");
}

bool checkIfShouldPing() {
  if (millis() - lastPingMillis >= PING_INTERVAL * 1000UL) {
    lastPingMillis = millis(); 
    sendPing();
    return true;
  }

  return false;
}