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

const char greeting[] = "Hello from ESP32-S3";

static void on_samples(int16_t *buf, unsigned count)
{
  // ESP_LOGI(TAG, "Samples: %i", count);
}

extern "C" void app_main()
{
  init_wifi();

  unsigned prio = uxTaskPriorityGet(NULL);

  if (picotts_init(prio, on_samples, TTS_CORE))
  {
    picotts_add(greeting, sizeof(greeting));

    vTaskDelay(pdMS_TO_TICKS(10000));

    picotts_shutdown();
  }
  else
  {
    printf("Failed to initialise TTS\n");
  }

  init_fs();
}
