#define ADC_PIN A5
#define ADC_RESOLUTION 1024  // 10 bit
#define VREF 5


void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);
}

void loop() {
  int potVal = analogRead(ADC_PIN);
  Serial.print("PotValue:");
  Serial.println(potVal);
  delay(200);
}
