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
#include "../lib/unravel.h"

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
const Song songNames[] = {
  { '1', "Never Gonna Give You Up", "Never Gonna",    "Give You Up", "Rick Astley",      playRickRoll,        stopRickRoll        },
  { '2', "Guile Theme (SF2)",       "Guiles Theme",    "",            "Street Fighter II", playSF2Guile,        stopSF2Guile        },
  { '3', "He's a Pirate",           "He's a Pirate",  "",            "Pirates of the Caribbean", playPirates,  stopPirates         },
  { 'A', "Ken Theme (SF2)",         "Kens Theme",      "",            "Street Fighter II", playSF2Ken,          stopSF2Ken          },
  { 'B', "Master of Puppets",       "Master of",      "Puppets",     "Metallica",        playMetallicaMaster, stopMetallicaMaster },
  { 'C', "The Trooper (IM)",        "The Trooper",    "",            "Iron Maiden",      playTrooper,         stopTrooper         },
};


const int song_count = sizeof(songNames) / sizeof(songNames[0]);

// Function prototypes
void colorLEDStable(int blue);
void stopTone();

int  songIndexForKey(char key);
void drawMenu();
void drawNowPlaying(int idx);
void stopTone();


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

void loop() {
  char key = keypad.getKey();

  if (key) {
    switch (key) {
      case '1':
        drawNowPlaying(0);
        playRickRoll(buzzerPin);
        stopRickRoll(buzzerPin);
        break;
      case '2':
        drawNowPlaying(1);
        playSF2Guile(buzzerPin);
        stopSF2Guile(buzzerPin);
        break;
      case '3':
        drawNowPlaying(2);
        playPirates(buzzerPin);
        stopPirates(buzzerPin);
        drawMenu();
        break;
      case 'A':
        drawNowPlaying(3);
        playSF2Ken(buzzerPin);
        stopSF2Ken(buzzerPin);
        drawMenu();
        break;
      case 'B':
        drawNowPlaying(4);
        playMetallicaMaster(buzzerPin);
        stopMetallicaMaster(buzzerPin);
        drawMenu();
        break;
      case 'C':
        drawNowPlaying(5);
        playTrooper(buzzerPin);
        stopTrooper(buzzerPin);
        drawMenu();
        break;
      case 'D':
        drawNowPlaying(6);
        playPaperWings(buzzerPin);
        stopPaperWings(buzzerPin);
        drawMenu();
        break;
      case '*':
        stopTone();
        break;
      default:
        colorLEDStable(0); // Off
        break;
    }
  }

}

// Function declarations
void colorLEDStable(int blue) {
  analogWrite(bluePin, blue);
}

int songIndexForKey(char key) {
  for (int i = 0; i < song_count; i++)
    if (songNames[i].key == key) return i;
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
    char badge[2] = { songNames[i].key, 0 };
    scr.drawString(badge, 3 + BADGE_W / 2, y + ROW_H / 2);

    // song title
    scr.setTextColor(COL_TEXT, COL_BG);
    scr.setTextDatum(ML_DATUM);
    scr.drawString(songNames[i].menu, 22, y + ROW_H / 2);
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
  scr.drawString(songNames[idx].l1, 80, 54);
  if (songNames[idx].l2[0] != 0)
    scr.drawString(songNames[idx].l2, 80, 66);

  // artist
  scr.drawString(songNames[idx].artist, 80, 90);

  scr.pushSprite(0, 0);
}

void stopTone() {
  ledcWriteTone(BUZZER_CH, 0); 
  i2s_zero_dma_buffer(I2S_PORT);
}

