#include <Arduino.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {15, 14, 13, 12}; // Row pins on ESP32
byte colPins[COLS] = {19, 23, 22, 21}; // Column pins on ESP32

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Function prototypes
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
  delay(3000); 
  Serial.println("Hello, ESP32!");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
  }

}

// Function declarations
int myFunction(int x, int y) {
  return x + y;
}