#pragma once

#include <Arduino.h>
#include <esp_log.h>
// #include <PDMOutput.h>
// #include <PDMTimerOutput.h>
#include <WAVFileReader.h>
#include <Audio.h>

#include "config.h"

#define PDM_GPIO_NUM 44
#define SAMPLE_RATE 16000

void audio_init();
void audio_play();
