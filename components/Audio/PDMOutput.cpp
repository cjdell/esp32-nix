
#include "PDMOutput.h"
#include <Arduino.h>
#include <driver/i2s_std.h>

void PDMOutput::start(uint32_t sample_rate)
{
    // i2s config for writing both channels of I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_PDM),
        .sample_rate = sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0};

    // install and start i2s driver
    i2s_driver_install(m_i2s_port, &i2s_config, 0, NULL);
    // set up the i2s pins
    i2s_set_pin(m_i2s_port, &m_i2s_pins);
    // clear the DMA buffers
    i2s_zero_dma_buffer(m_i2s_port);

    i2s_start(m_i2s_port);
}
