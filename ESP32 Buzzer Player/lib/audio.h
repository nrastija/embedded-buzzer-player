// audio.h
#pragma once
#include <Arduino.h>

#define SAMPLE_RATE  44100      // or 16000, must match initAudio()
#define VOLUME       1000
#define TABLE_SIZE   256

#define I2S_PORT I2S_NUM_0

extern int16_t sineTable[TABLE_SIZE];

void initSine();
void initAudio();
void playTone(int freq, int ms);