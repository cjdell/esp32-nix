#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <picotts.h>

#include "wifi.h"
#include "fs.h"
#include "audio.h"

#define TTS_CORE 1
#define LED 21

const char greeting[] = "Hello from ESP32-S3";

static void on_samples(int16_t *buf, unsigned count)
{
  // ESP_LOGI(TAG, "Samples: %i", count);
}

void setup()
{
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  ESP_LOGI("main", "PSRAM SIZE: %i", (int)heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
  ESP_LOGI("main", "PSRAM FREE: %i", (int)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

  // wifi_init();

  fs_init();

  audio_init();

  pinMode(LED, OUTPUT);
}

int i = 0;

void loop()
{
  audio_play();
  vTaskDelay(1);

  digitalWrite(LED, i % 2 == 1 ? HIGH : LOW);
  i++;

  // while (true)
  // {
  //   digitalWrite(LED, HIGH);
  //   delay(500);
  //   digitalWrite(LED, LOW);
  //   delay(500);
  // }
}

void audio_info(const char *info)
{
  ESP_LOGI("main", "==== INFO: %s", info);
}

// extern "C" void app_main()
// {
//   wifi_init();

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

//   fs_init();
// }
