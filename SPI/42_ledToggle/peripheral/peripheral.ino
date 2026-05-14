#include <SPI.h>

#define LED_PIN 2

uint8_t g_currentLEDVal = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SS, INPUT);

  // enable
  SPCR |= _BV(SPE) | _BV(SPIE);

  // spi mode 2
  SPCR |= _BV(CPOL);
  SPCR &= ~_BV(CPHA);

  // peripheral mode
  SPCR &= ~_BV(MSTR);
}

ISR(SPI_STC_vect) {
  digitalWrite(LED_PIN, !g_currentLEDVal);
  Serial.println("changing led state");
  g_currentLEDVal = !g_currentLEDVal;
}

void loop() {
}
