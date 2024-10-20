#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <string.h>
#include <driver/sdm.h>
#include <esp_spiffs.h>
#include <esp32-hal-log.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <picotts.h>

#include "wifi.h"
#include "fs.h"

#define TTS_CORE 1
#define LED 21

const char greeting[] = "Hello from ESP32-S3";

static void on_samples(int16_t *buf, unsigned count)
{
  // ESP_LOGI(TAG, "Samples: %i", count);
}

void setup()
{
  init_wifi();

  pinMode(LED, OUTPUT);
}

void loop()
{
  init_fs();

  while (true)
  {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}

// extern "C" void app_main()
// {
//   init_wifi();

//   unsigned prio = uxTaskPriorityGet(NULL);

//   if (picotts_init(prio, on_samples, TTS_CORE))
//   {
//     picotts_add(greeting, sizeof(greeting));

//     vTaskDelay(pdMS_TO_TICKS(10000));

//     picotts_shutdown();
//   }
//   else
//   {
//     printf("Failed to initialise TTS\n");
//   }

//   init_fs();
// }
