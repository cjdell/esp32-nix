#include "picotts.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <driver/sdm.h>
#include <esp_spiffs.h>
#include <esp32-hal-log.h>

#define TTS_CORE 1

static const char *TAG = "esp32-s3-nix";

const char greeting[] = "Hello from ESP32-S3";

static void on_samples(int16_t *buf, unsigned count)
{
  // ESP_LOGI(TAG, "Samples: %i", count);
}

// https://github.com/espressif/esp-idf/blob/master/examples/storage/spiffs/main/spiffs_example_main.c
void check_fs()
{
  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true};

  // Use settings defined above to initialize and mount SPIFFS filesystem.
  // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    }
    else if (ret == ESP_ERR_NOT_FOUND)
    {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    }
    else
    {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
    esp_spiffs_format(conf.partition_label);
    return;
  }
  else
  {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }

  // Open file for reading
  ESP_LOGI(TAG, "Reading file");
  auto f = fopen("/spiffs/hello.txt", "r");
  if (f == NULL)
  {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return;
  }
  char line[64];
  fgets(line, sizeof(line), f);
  fclose(f);
  // strip newline
  char *pos = strchr(line, '\n');
  if (pos)
  {
    *pos = '\0';
  }
  ESP_LOGI(TAG, "Read from file: '%s'", line);
}

extern "C" void app_main()
{
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

  check_fs();
}
