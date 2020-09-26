#include "station.h"

HTTPClient http;

bool setupWiFi() {
  int wifiRetriesLeft = WIFI_CONNECT_RETRIES;

  #ifndef PRECONFIGURED
  char ssid[60];
  readFromEEPROM(ssid, "ssid");
  char password[60];
  readFromEEPROM(password, "password");
  #else
  char ssid[60] = CFG_WIFI_SSID;
  char password[60] = CFG_WIFI_PASSWORD;
  #endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && wifiRetriesLeft > 0) {
    delay(500);
    ardprintf("Station: Connecting with SSID %s", ssid);
    ardprintf("Station: Connecting with password %s", password);

    wifiRetriesLeft -= 1;
  }

  if (wifiRetriesLeft <= 0 || WiFi.status() != WL_CONNECTED) {
    ardprintf("Station: Could not connect to WiFi.");
    return false;
  }
  
  ardprintf("Station: Connected to WiFi");

  http.begin("http://" + String(CFG_SENSOR_DASHBOARD_URL) + "/api/measurements/multi");
  return true;
}

bool makeNetworkRequest(const char * url, const char * authorization, const char * content) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", authorization);
  int httpResponseCode = http.POST(content);

  if (httpResponseCode > 0) {
    ardprintf("Station: HTTP Response code: %d", httpResponseCode);
    const char * payload = http.getString().c_str();
    ardprintf("%s", payload);
    http.end();
    return true;
  }
  else {
    http.end();
    ardprintf("Station: Error code: %d", httpResponseCode);
    return false;
  }

  return false;
}