#include <SPI.h>

#define BUTTON_PIN 2

uint8_t prevButtonState = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE2));
  digitalWrite(SS, HIGH);
}

// requires software debounce if no hardware debounce in place
bool buttonIsPressed() {
  uint8_t val = digitalRead(BUTTON_PIN);

  if (val == LOW && prevButtonState == HIGH) {
    Serial.println("Button press detected");
    prevButtonState = LOW;
    return true;
  } else if (val == HIGH && prevButtonState == LOW) {
    Serial.println("Button release detected");
    prevButtonState = HIGH;
  }
  return false;
}

void loop() {
  if (buttonIsPressed()) {
    digitalWrite(SS, LOW);
    SPI.transfer(0);
    digitalWrite(SS, HIGH);
  }
}
