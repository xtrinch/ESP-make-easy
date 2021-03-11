#include "sleep.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <esp_wifi.h>
#include <esp_sleep.h>


void goToSleep() {
  ardprintf("Sleep: Every %d sec", CFG_SLEEP_SECONDS);

  #ifdef ESP32
  esp_sleep_enable_timer_wakeup(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR);
  // shut practically everything off
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  #endif

  Serial.flush();

  #ifdef ESP32
  // disables the brownout, apparently :)
  CLEAR_PERI_REG_MASK(RTC_CNTL_BROWN_OUT_REG, RTC_CNTL_BROWN_OUT_RST_ENA);
  esp_wifi_stop();
  esp_deep_sleep_start();
  #elif ESP8266
  ESP.deepSleep(CFG_SLEEP_SECONDS * uS_TO_S_FACTOR, WAKE_RFCAL);
  #endif

  ardprintf("This will never be printed");
}