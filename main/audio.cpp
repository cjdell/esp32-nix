#include "audio.h"

// AudioOutput *audioOutput = NULL;
WAVFileReader *wavFileReader = NULL;
Audio audio;

void audio_init()
{
    // ESP_LOGI(TAG, "Init PDM...");

    // i2s_pin_config_t i2s_pdm_pins = {
    //     // no bck for PDM
    //     .bck_io_num = I2S_PIN_NO_CHANGE,
    //     // use a dummy pin for the LR clock - 45 or 46 is a good options
    //     // as these are normally not connected
    //     .ws_io_num = 45,
    //     // where should we send the PDM data
    //     .data_out_num = PDM_GPIO_NUM,
    //     // no data to read
    //     .data_in_num = I2S_PIN_NO_CHANGE};

    // audioOutput = new PDMOutput(i2s_pdm_pins);

    // // audioOutput = new PDMTimerOutput(GPIO_NUM_44);

    ESP_LOGI("audio", "Configure...");

    audio.setPinout(45, 46, 44);
    audio.setVolume(21); // default 0...21

    ESP_LOGI("audio", "SPIFFS...");

    SPIFFS.begin(false, "/spiffs", 10, "storage");

    delay(1000);

    ESP_LOGI("audio", "Connect...");

    // audio.connecttoFS(SPIFFS, "sample.wav");
    audio.connecttoFS(SPIFFS, "one.ogg");
}

void audio_play()
{
    // ESP_LOGI("audio", "Loop...");

    audio.loop();

    // ESP_LOGI(TAG, "Loading WAV file...");
    // FILE *fp = fopen("/spiffs/sample.wav", "r");

    // if (fp == NULL)
    // {
    //     ESP_LOGE(TAG, "Failed to open file for reading");
    //     return;
    // }

    // wavFileReader = new WAVFileReader(fp);

    // audioOutput->start(SAMPLE_RATE);

    // while (true)
    // {
    //     int16_t samples[1000];
    //     int count = wavFileReader->read(samples, 1000);
    //     if (count > 0)
    //     {
    //         audioOutput->write(samples, count);
    //     }
    //     else
    //     {
    //         wavFileReader->rewind();
    //     }
    // }
}
