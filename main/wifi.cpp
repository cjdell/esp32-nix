#include "wifi.h"

void init_wifi()
{
  delay(5000);

  pinMode(0, INPUT);
  digitalWrite(0, HIGH); // Pull-up

  if (digitalRead(0) == LOW)
  {
    ESP_LOGW(TAG, "==== ERASING FLASH ====");
    ESP_ERROR_CHECK(nvs_flash_erase());
  }

  delay(5000);

  nvs_flash_init();

  delay(1000);

  ESP_LOGI(TAG, "WiFiManager starting...");

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("ESP32-S3"); // password protected ap

  if (!res)
  {
    ESP_LOGE(TAG, "Failed to connect");

    ESP_LOGW(TAG, "==== ERASING FLASH IN 5 SECONDS ====");

    delay(5000);

    ESP_ERROR_CHECK(nvs_flash_erase());

    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    ESP_LOGI(TAG, "connected...yeey :)");
  }
}
