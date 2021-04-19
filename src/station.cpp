#include "station.h"

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
  ardprintf("Station: Connecting with %s:%d", ssid, password);

  while (WiFi.status() != WL_CONNECTED && wifiRetriesLeft > 0) {
    delay(100);
    wifiRetriesLeft -= 1;
  }

  if (wifiRetriesLeft <= 0 || WiFi.status() != WL_CONNECTED) {
    ardprintf("Station: Could not connect to WiFi.");
    return false;
  }
  
  ardprintf("Station: Connected to WiFi");

  return true;
}

int makeSecureNetworkRequest(const char * url, const char * authorization, const char * content, const char * response, const char * method, const char * certificate) {
  WiFiClientSecure client;
  if (!certificate) {
    #ifndef ESP32S2
    #ifdef ESP32
      client.setCACert((const char *)certificate_start);
    #elif defined ESP8266
      client.setInsecure();
    #endif
    #endif
  } else {
    client.setCACert((const char *)certificate);
  }

  client.connect(url, 443);
  HTTPClient http;

  http.begin(client, url);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", authorization);
  http.addHeader("Forwarder", CFG_ACCESS_TOKEN);
  int httpResponseCode = -1;
  if (strcmp(method, "POST") == 0) {
    httpResponseCode = http.POST(content);
  } else {
    httpResponseCode = http.GET();
  }

  if (httpResponseCode > 0) {
    ardprintf("Station: HTTPS %d", httpResponseCode);
    const char * payload = http.getString().c_str();
    // ardprintf("%s", payload);
    if (response != NULL) {
      strcpy((char *)response, payload);
    }
    http.end();
  }
  else {
    http.end();
    ardprintf("Station: Error code: %d", httpResponseCode);
  }

  return httpResponseCode;
}

bool makeNetworkRequest(const char * url, const char * authorization, const char * content, const char * response, const char * method) {
  HTTPClient http;
  http.begin(url);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", authorization);
  http.addHeader("Forwarder", CFG_ACCESS_TOKEN);

  int httpResponseCode;
  if (strcmp(method, "POST") == 0) {
    httpResponseCode = http.POST(content);
  } else {
    httpResponseCode = http.GET();
  }

  if (httpResponseCode > 0) {
    ardprintf("Station: HTTP %d", httpResponseCode);
    const char * payload = http.getString().c_str();
    // ardprintf("%s", payload);
    if (response != NULL) {
      strcpy((char *)response, payload);
    }
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