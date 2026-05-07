
#define ADC_PIN A5
#define ADC_RESOLUTION 1024

uint8_t mode = DEFAULT;

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t potVal = analogRead(ADC_PIN);
  static uint16_t statVal = 500;

  Serial.print("StatVal: ");
  Serial.print(statVal);
  Serial.print(", PotValue: ");
  Serial.println(potVal);
  delay(500);
}
