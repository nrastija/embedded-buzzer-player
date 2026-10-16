#include <Arduino.h>
#include <Keypad.h>
#include "../lib/rickroll.h"

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

// Function prototypes
void colorRGBLED(int red, int green, int blue);
void playTone(int freq, int ms);
void stopTone();

void setup() {
  Serial.begin(115200);
  delay(3000); 
  Serial.println("Hello, ESP32!");
  
  ledcSetup(BUZZER_CH, 2400, 8);  
  ledcAttachPin(buzzerPin, BUZZER_CH);

  for (int i=0; i<3; i++) {
    colorRGBLED(100, 100, 100); 
    delay(500);
    colorRGBLED(0, 0, 0);
    delay(500);
  }
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    switch (key) {
      case '1':
        //play buzzer sound
        playRickRoll(buzzerPin);
        stopRickRoll(buzzerPin);
        break;
      case '2':
        colorRGBLED(0, 255, 0); // Green
        break;
      case '3':
        colorRGBLED(0, 0, 255); // Blue
        break;
      case 'A':
        colorRGBLED(255, 255, 0); // Yellow
        break;
      case 'B':
        colorRGBLED(0, 255, 255); // Cyan
        break;
      case 'C':
        colorRGBLED(255, 0, 255); // Magenta
        break;
      case 'D':
        colorRGBLED(255, 255, 255); // White
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
void colorRGBLED(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void playTone(int freq, int ms) {
  ledcWriteTone(BUZZER_CH, freq);
  delay(ms);
  ledcWriteTone(BUZZER_CH, 0);   // stop
}

void stopTone() {
  ledcWriteTone(BUZZER_CH, 0);
}