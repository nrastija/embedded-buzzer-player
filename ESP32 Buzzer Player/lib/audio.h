// audio.h
#pragma once
#include <Arduino.h>
#include <driver/i2s.h>
#include "../lib/notes.h"

#define SAMPLE_RATE  44100      // or 16000, must match initAudio()
#define VOLUME       1000
#define TABLE_SIZE   256

#define I2S_PORT I2S_NUM_0

extern int16_t sineTable[TABLE_SIZE];

void initSine();
void initAudio();
void playTone(int freq, int ms);
void stopTone();

void loadNote(Track &t);
void initTrack(Track &t, const Note* notes, uint16_t count);
int16_t nextSample(Track &t);
