#include <SPI.h>

#define RANDOM_DATA 0xFF  // master is just reading

void setup() {
  Serial.begin(9600);

  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE2));
  digitalWrite(SS, HIGH);
}

bool buttonIsPressed() {
  // simulate button press every second
  delay(1000);
  return true;
}

void loop() {
  if (buttonIsPressed()) {
    digitalWrite(SS, LOW);
    uint8_t received = SPI.transfer(RANDOM_DATA);
    digitalWrite(SS, HIGH);
    if (received > 0 && received < 7) {  // valid dice value
      Serial.print("dice value: ");
      Serial.println(received);
    } else {
      Serial.println("error: try again and check spi connections");
    }
  }
}

// slave
// void setup() {
//   pinMode(MISO, OUTPUT);
//   pinMode(SS, INPUT_PULLUP);
//
//   SPCR |= _BV(SPE);
//
//   SPI.setDataMode(SPI_MODE2);
//   SPI.attachInterrupt();
//
//   // generate random number and store in data register
//   SPDR = random(1, 7);
// }
//
// // serial transfer complete routine
// ISR(SPI_STC_vect) {
//   SPDR = random(1, 7);
// }
//
// void loop() {
// }
