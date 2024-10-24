#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include <PDMOutput.h>
// #include <PDMTimerOutput.h>
#include <WAVFileReader.h>
#include <Audio.h>

#include "config.h"

#define PDM_GPIO_NUM 44
#define SAMPLE_RATE 16000

void audio_file_init();
void audio_file_play();

void audio_pdm_init();
void audio_pdm_play();
void audio_pdm_start(uint32_t sample_rate);
void audio_pdm_send_samples(int16_t samples[], int count);
