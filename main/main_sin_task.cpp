#include <Arduino.h>
#include <driver/i2c.h>
#include <driver/i2s_common.h>
#include <audio_codec_data_if.h>
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>
#include <esp_codec_dev_defaults.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"

static const char *TAG = "esp32-s3-nix";

static i2s_chan_handle_t i2s_tx_chan = NULL;

#define SAMPLE_RATE 48000 // Sample rate in Hz
#define FREQUENCY 440     // Frequency of the sine wave in Hz
#define BUFFER_SIZE 256   // Buffer size for samples

RingbufHandle_t ringbuf = NULL; // Handle for the ring buffer

int total_bytes_sent = 0;
int total_bytes_recv = 0;

int count = 0;

void buffer_generation_task(void *param);
void i2s_write_task(void *param);

void setup()
{
    esp_err_t ret;

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

    ret = i2s_new_channel(&chan_cfg, &i2s_tx_chan, NULL);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "i2s_new_channel failed");
    }

    const i2s_pdm_tx_config_t pdm_tx_cfg = {
        .clk_cfg = I2S_PDM_TX_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .sd_prescale = 0,
            .sd_scale = I2S_PDM_SIG_SCALING_MUL_1,
            .hp_scale = I2S_PDM_SIG_SCALING_DIV_2,
            .lp_scale = I2S_PDM_SIG_SCALING_MUL_1,
            .sinc_scale = I2S_PDM_SIG_SCALING_MUL_1,
            .line_mode = I2S_PDM_TX_ONE_LINE_CODEC,
            .hp_en = true,
            .hp_cut_off_freq_hz = 35.5,
            .sd_dither = 0,
            .sd_dither2 = 1,
        },
        .gpio_cfg = {
            .clk = GPIO_NUM_45,
            .dout = GPIO_NUM_44,
        },
    };

    ret = i2s_channel_init_pdm_tx_mode(i2s_tx_chan, &pdm_tx_cfg);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "i2s_channel_init_pdm_tx_mode failed");
    }

    ret = i2s_channel_enable(i2s_tx_chan);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "i2s_channel_enable failed");
    }

    // Create a ring buffer for inter-task communication
    ringbuf = xRingbufferCreate(BUFFER_SIZE * sizeof(int16_t) * 4, RINGBUF_TYPE_BYTEBUF);
    if (ringbuf == NULL)
    {
        ESP_LOGE(TAG, "Failed to create ring buffer");
    }

    // Create tasks
    xTaskCreatePinnedToCore(buffer_generation_task, "BufferGenTask", 2048, NULL, 1, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(i2s_write_task, "I2SWriteTask", 2048, NULL, 1, NULL, APP_CPU_NUM);
}

void loop()
{
    // Empty loop since tasks are handling everything
    vTaskDelay(1 / portTICK_PERIOD_MS);
}

void buffer_generation_task(void *param)
{
    static float phase = 0.0f; // Phase accumulator
    while (1)
    {
        int16_t *buffer = (int16_t *)malloc(BUFFER_SIZE * sizeof(int16_t));
        if (buffer == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for buffer");
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }

        // Generate the sine wave samples
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            buffer[i] = (int16_t)(32767 * sinf(phase));
            phase += 2.0f * PI * FREQUENCY / SAMPLE_RATE;
            if (phase >= 2.0f * PI)
            {
                phase -= 2.0f * PI;
            }
        }

        printf("I");

        total_bytes_sent += BUFFER_SIZE * sizeof(int16_t);

        // Add buffer to ring buffer
        xRingbufferSend(ringbuf, (void *)buffer, BUFFER_SIZE * sizeof(int16_t), portMAX_DELAY);

        free(buffer); // Free the buffer after sending to ring buffer

        // Optionally, you can yield to allow other tasks to run
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void i2s_write_task(void *param)
{
    while (1)
    {
        size_t item_size;
        int16_t *buffer = (int16_t *)xRingbufferReceive(ringbuf, &item_size, portMAX_DELAY);
        if (buffer != NULL)
        {
            // Write the samples to the I2S channel
            size_t bytes_written = 0;

            printf("O");

            total_bytes_recv += item_size;
            count++;

            if (count % 100 == 0)
            {
                printf("Sent: %i / Recv: %i\n", total_bytes_sent, total_bytes_recv);
            }

            int ret = i2s_channel_write(i2s_tx_chan, buffer, item_size, &bytes_written, portMAX_DELAY);
            if (ret != ESP_OK)
            {
                ESP_LOGE(TAG, "i2s_channel_write failed");
            }
            else if (bytes_written != item_size)
            {
                ESP_LOGE(TAG, "Incomplete I2S write");
            }

            // Return the buffer to the ring buffer to free memory
            vRingbufferReturnItem(ringbuf, (void *)buffer);

            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to receive buffer from ring buffer");
        }
    }
}
