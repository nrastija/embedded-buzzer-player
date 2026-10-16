#include <Keypad.h>
#include "../lib/audio.h"

#include "../lib/rickroll.h"
#include "../lib/sf2_guile.h"
#include "../lib/pirates.h"
#include "../lib/sf2_ken.h"
#include "../lib/metallica_master.h"

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
const int bluePin = 23;   // Red LED pin
const int greenPin = 21; // Green LED pin
const int redPin = 22;  // Blue LED pin

// Buzzer configuration
const int buzzerPin = 33; // Buzzer pin
const int BUZZER_CH = 4; 

int16_t sineTable[TABLE_SIZE];

// Function prototypes
void colorRGBLED(int red, int green, int blue);
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

  for (int i=0; i<3; i++) {
    colorRGBLED(100, 100, 100); 
    delay(500);
    colorRGBLED(0, 0, 0);
    delay(500);
  }

  colorRGBLED(0, 0, 100);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
  
    switch (key) {
      case '1':
        colorRGBLED(0, 100, 0); // red -> song is playing 
        playRickRoll(buzzerPin);
        stopRickRoll(buzzerPin);
        colorRGBLED(0, 0, 100); // back to green -> song is stopped
        break;
      case '2':
        colorRGBLED(0, 100, 0); 
        playSF2Guile(buzzerPin);
        stopSF2Guile(buzzerPin);
        colorRGBLED(0, 0, 100); 
        break;
      case '3':
        colorRGBLED(0, 100, 0);
        playPirates(buzzerPin);
        stopPirates(buzzerPin);
        colorRGBLED(0, 0, 100);
        break;
      case 'A':
        colorRGBLED(0, 100, 0);
        playSF2Ken(buzzerPin);
        stopSF2Ken(buzzerPin);
        colorRGBLED(0, 0, 100);
        break;
      case 'B':
        colorRGBLED(0, 100, 0);
        playMetallicaMaster(buzzerPin);
        stopMetallicaMaster(buzzerPin);
        colorRGBLED(0, 0, 100);
        break;
      case 'C':
        colorRGBLED(0, 100, 0);
        // placeholder for song 6
        colorRGBLED(0, 100, 0);
        break;
      case 'D':
        colorRGBLED(0, 100, 0);
        // placeholder for song 7
        colorRGBLED(0, 0, 100);
        break;
      case '*':
        stopTone();
        break;
      default:
        colorRGBLED(0, 0, 0); // Off
        break;
    }
  }

}

// Function declarations
void colorRGBLED(int green, int blue, int red) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void stopTone() {
  ledcWriteTone(BUZZER_CH, 0); // Stop the tone
  i2s_zero_dma_buffer(I2S_PORT);
}