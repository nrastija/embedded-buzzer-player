// audio.h
#pragma once
#include <Arduino.h>
#include <driver/i2s.h>
#include "../lib/notes.h"

#define SAMPLE_RATE  44100      
#define VOLUME       3000
#define TABLE_SIZE   256

#define I2S_PORT I2S_NUM_0

extern int16_t sineTable[TABLE_SIZE];
extern volatile bool stopRequested;

void initSine();
void initAudio();
void playTone(int freq, int ms);
void stopTone();
bool checkStop();

void loadNote(Track &t);
void initTrack(Track &t, const Note* notes, uint16_t count);
int16_t nextSample(Track &t);
