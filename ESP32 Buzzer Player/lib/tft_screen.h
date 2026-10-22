#pragma once

#define COL_BG     TFT_BLACK
#define COL_ACCENT TFT_CYAN
#define COL_TEXT   TFT_WHITE
#define COL_SUB    TFT_DARKGREY


struct Song {
  char key;
  const char* menu;
  const char* l1;
  const char* l2;
  const char* artist;
  void (*play)(int);
  void (*stop)(int);
};

