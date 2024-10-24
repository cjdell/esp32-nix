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

void speak();

void setup()
{
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  ESP_LOGI("main", "PSRAM SIZE: %i", (int)heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
  ESP_LOGI("main", "PSRAM FREE: %i", (int)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

  // wifi_init();

  fs_init();

  audio_pdm_init();

  pinMode(LED, OUTPUT);

  speak();
}

int i = 0;

void loop()
{
  // audio_pdm_play();

  vTaskDelay(1000);

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

int16_t *buffer;
int16_t *buffer_pos = NULL;

void task_send_samples(void *pvParameters)
{
  // auto buffer2 = (int16_t *)malloc(1024 * 2);

  // audio_pdm_send_samples(buffer2, 1024);

  // buffer2 = (int16_t *)malloc(1024 * 2);
}

static void on_samples(int16_t *buf, unsigned count)
{
  // memcpy(&buffer_pos, buf, count);

  buffer_pos += count * 2;

  if (buffer_pos >= buffer + 1024)
  {
    ESP_LOGI(TAG, "Sending... %i", buffer_pos - buffer);

    buffer_pos = buffer;

    // audio_pdm_send_samples(buffer, 1024);

    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
    // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
    // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
    // the new task attempts to access it.
    xTaskCreate(task_send_samples, "task_send_samples", 2048, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle);
  }
}

void speak()
{
  buffer = (int16_t *)malloc(1024 * 2);
  buffer_pos = buffer;

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

  // fs_init();
}
