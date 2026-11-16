#include <Keypad.h>
#include <SPI.h>
#include <TFT_eSPI.h> 

// included once (key includes)
#include "../lib/audio.h"
#include "../lib/tft_screen.h"

// includes for songs
#include "../lib/rickroll.h"
#include "../lib/sf2_guile.h"
#include "../lib/pirates.h"
#include "../lib/sf2_ken.h"
#include "../lib/metallica_master.h"
#include "../lib/trooper.h"

#include "../lib/blue_bird.h"
#include "../lib/tes_v_skyrim.h"
#include "../lib/megalovania.h"
#include "../lib/babyshark.h"
#include "../lib/previse_suza.h"
#include "../lib/mata.h"

// 4x4 Matrix Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {15, 14, 13, 12}; // Row pins on ESP32
byte colPins[COLS] = {26, 27, 18, 19}; // Column pins on ESP32

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// RGB LED configuration
const int bluePin = 21; // Blue LED pin

// Buzzer configuration
const int buzzerPin = 33; // Buzzer pin
const int BUZZER_CH = 4; 

// Sine wave table configuration
int16_t sineTable[TABLE_SIZE];

// TFT display configuration
TFT_eSPI tft;
TFT_eSprite scr(&tft);   

// Songs for TFT display
const Song songNames1[] = {
  { '1', "Never Gonna Give You Up", "Never Gonna",    "Give You Up", "Rick Astley",      playRickRoll,        stopRickRoll        },
  { '2', "Guile Theme (SF2)",       "Guiles Theme",    "",            "Street Fighter II", playSF2Guile,        stopSF2Guile        },
  { '3', "He's a Pirate",           "He's a Pirate",  "",            "Pirates of the Caribbean", playPirates,  stopPirates         },
  { 'A', "Ken Theme (SF2)",         "Kens Theme",      "",            "Street Fighter II", playSF2Ken,          stopSF2Ken          },
  { 'B', "Master of Puppets",       "Master of",      "Puppets",     "Metallica",        playMetallicaMaster, stopMetallicaMaster },
  { 'C', "The Trooper",        "The Trooper",    "",            "Iron Maiden",      playTrooper,         stopTrooper         },
};

const Song songNames2[] = {
  { '1', "Blue Bird", "Blue Bird ",    "Naruto :)", "Ikimonogakari",      playBlueBird,        stopBlueBird        },
  { '2', "TES V: Skyrim",       "Skyrim ",    "Dragonborn!",            "Jeremy Soule", playTESVSkyrim,        stopTESVSkyrim        },
  { '3', "Megalovania",           "Undertale :D",  "",            "Toby Fox", playMegalovania,  stopMegalovania        },
  { 'A', "Previse suza u mom pivu",         "Prljavo",      "Kazaliste",            "PLATI MI PIVU", playPreviseSuza,          stopPreviseSuza          },
  { 'B', "Mata",       "SLAVONIJA",      "Miroslav",     "Skoro",        playMata, stopMata },
  { 'C', "Baby Shark",        "BABY SHARK",    "dooo dooo dooo doooooo",            "Pinkfong",      playBabyShark,         stopBabyShark         },
};


const int song_count = sizeof(songNames1) / sizeof(songNames1[0]);
volatile bool stopRequested = false;

// Function prototypes
void colorLEDStable(int blue);
void stopTone();

int  songIndexForKey(char key);
void drawMenu();
void drawNowPlaying(int idx);
void finishPlayback();



void setup() {
  Serial.begin(115200);
  delay(3000); 
  Serial.println("Hello, ESP32!");
  
  ledcSetup(BUZZER_CH, 2400, 8);  
  ledcAttachPin(buzzerPin, BUZZER_CH);

  // I2S configuration
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  cfg.sample_rate = 44100;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  cfg.dma_buf_count = 8;
  cfg.dma_buf_len = 64;
  cfg.tx_desc_auto_clear = true;

  i2s_pin_config_t pins = {};
  pins.mck_io_num = I2S_PIN_NO_CHANGE;
  pins.bck_io_num = 33;   // BCLK pin
  pins.ws_io_num = 25;    // LRC pin
  pins.data_out_num = 32; // DIN pin
  pins.data_in_num = I2S_PIN_NO_CHANGE;

  i2s_driver_install(I2S_PORT, &cfg, 0, NULL);
  i2s_set_pin(I2S_PORT, &pins);
  
  for (int i = 0; i < TABLE_SIZE; i++)
    sineTable[i] = (int16_t)(sin(2.0 * PI * i / TABLE_SIZE) * VOLUME);

  // screen configuration
  tft.init();
  tft.setRotation(1);        
  scr.createSprite(160, 128); 

  drawMenu();

  for (int i=0; i<3; i++) {
    colorLEDStable(100); 
    delay(500);
    colorLEDStable(0);
    delay(500);
  }

  colorLEDStable(100);
}

int current_list = 1;

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == '#') {
    current_list = (current_list == 1) ? 2 : 1;
    drawMenu();
    return;
  }

  const Song* list = (current_list == 1) ? songNames1 : songNames2;

  int idx = songIndexForKey(key);       
  if (idx < 0) return;                  

  stopRequested = false;
  drawNowPlaying(idx);
  list[idx].play(buzzerPin);
  finishPlayback();
}


// Function declarations
void colorLEDStable(int blue) {
  analogWrite(bluePin, blue);
}

int songIndexForKey(char key) {
  for (int i = 0; i < song_count; i++)
    if (current_list == 1 && songNames1[i].key == key) return i;
    else if (current_list == 2 && songNames2[i].key == key) return i;
  return -1;
}

void drawMenu() {
  scr.fillSprite(COL_BG);

  scr.fillRoundRect(0, 0, 160, 28, 5, COL_ACCENT);
  scr.setTextDatum(MC_DATUM);
  scr.setTextColor(COL_BG, COL_ACCENT);
  scr.setTextFont(2);                 
  scr.drawString("ESP32 Buzzer Player", 80, 14);

  const int ROW_Y0  = 32;
  const int ROW_H   = 14;
  const int BADGE_W  = 16;

  for (int i = 0; i < song_count; i++) {
    int y = ROW_Y0 + i * ROW_H;

    // key badge
    scr.fillRoundRect(3, y + 1, BADGE_W, ROW_H - 3, 3, COL_ACCENT);
    scr.setTextColor(COL_BG, COL_ACCENT);
    scr.setTextFont(1);
    scr.setTextDatum(MC_DATUM);
    char badge[2];
    if (current_list == 1) {
      badge[0] = songNames1[i].key;
    } else {
      badge[0] = songNames2[i].key;
    }
    badge[1] = 0;
    scr.drawString(badge, 3 + BADGE_W / 2, y + ROW_H / 2);

    // song title
    scr.setTextColor(COL_TEXT, COL_BG);
    scr.setTextDatum(ML_DATUM);
    if (current_list == 1)  scr.drawString(songNames1[i].menu, 22, y + ROW_H / 2);
    else  scr.drawString(songNames2[i].menu, 22, y + ROW_H / 2);

  }

  // footer
  scr.setTextColor(COL_SUB, COL_BG);
  scr.setTextDatum(MC_DATUM);
  scr.setTextFont(1);
  scr.drawString("key: A-F : play | * : stop", 80, 122);
 
  scr.pushSprite(0, 0);               
}

void drawNowPlaying(int idx) {
  scr.fillSprite(COL_BG);
  scr.setTextDatum(MC_DATUM);

  scr.setTextColor(COL_ACCENT);
  scr.setTextFont(4);                 
  scr.drawString("\xEF\xBF\xAE", 80, 30);  

  // song title
  scr.setTextDatum(7);
  scr.setTextColor(COL_SUB);
  scr.setTextFont(1);
  scr.drawString("NOW PLAYING:", 80, 16);
  if (current_list == 1 ){
    scr.drawString(songNames1[idx].l1, 80, 54);
    if (songNames1[idx].l2[0] != 0)
    scr.drawString(songNames1[idx].l2, 80, 66);

    scr.drawString(songNames1[idx].artist, 80, 90);
  } 
  else {
    scr.drawString(songNames2[idx].l1, 80, 54);
    if (songNames2[idx].l2[0] != 0)
    scr.drawString(songNames2[idx].l2, 80, 66);

    scr.drawString(songNames2[idx].artist, 80, 90);

  }

  scr.pushSprite(0, 0);
}

void finishPlayback() {

  if (stopRequested) {
    scr.fillSprite(COL_BG);
    scr.setTextDatum(MC_DATUM);
    scr.setTextColor(COL_ACCENT);
    scr.setTextFont(2);
    scr.drawString("Stopped", 80, 64);
    scr.pushSprite(0, 0);
    delay(600);
  }

  drawMenu();
  stopRequested = false;
}

bool checkStop() {
  if (keypad.getKey() == '*') stopRequested = true;
  return stopRequested;
}