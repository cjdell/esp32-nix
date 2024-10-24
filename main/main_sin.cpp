#include <Arduino.h>
#include <driver/i2c.h>
#include <driver/i2s_common.h>
#include <audio_codec_data_if.h>
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>
#include <esp_codec_dev_defaults.h>
#include <math.h>

static const char *TAG = "esp32-s3-nix";

static i2s_chan_handle_t i2s_tx_chan = NULL;

#define SAMPLE_RATE 48000 // Sample rate in Hz
#define FREQUENCY 440     // Frequency of the sine wave in Hz (measured at 477Hz)
#define BUFFER_SIZE 256   // Buffer size for samples

int16_t buffer[BUFFER_SIZE]; // Buffer to hold the samples
static float phase = 0.0f;   // Phase accumulator for sine wave generation

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
}

auto seconds = 0;

void loop()
{
    size_t bytes_written = 0;

    // Generate the sine wave samples
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = (int16_t)(32767 * sinf(phase));

        // Add another tone at 2x freq every other second
        // if ((millis() / 1000) % 2 == 0)
        // {
        //     buffer[i] += (int16_t)(32767 * sinf(phase * 2.0f));
        // }

        phase += 2.0f * PI * FREQUENCY / SAMPLE_RATE;
        if (phase >= 2.0f * PI)
        {
            phase -= 2.0f * PI;
        }
    }

    // Write the samples to the I2S channel
    int ret = i2s_channel_write(i2s_tx_chan, buffer, BUFFER_SIZE * sizeof(int16_t), &bytes_written, 1000);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "i2s_channel_write failed");
    }
}
