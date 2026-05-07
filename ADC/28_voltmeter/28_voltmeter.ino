#define ADC_PIN A5
#define ADC_RESOLUTION 1024

uint8_t mode = DEFAULT;
float vRef = 5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t potVal = analogRead(ADC_PIN);
  float volts = (float)potVal / (ADC_RESOLUTION - 1) * vRef;
  Serial.print("val: ");
  Serial.print(potVal);
  Serial.print(" volts: ");
  Serial.println(volts);

  // updating the reference voltagresolutione to be more precise for smaller voltages
  if (volts < 1.1 && vRef != 1.1) {
    Serial.println("Updating analog reference to internal");
    analogReference(INTERNAL);
    vRef = 1.1;

  } else if (volts >= 1.1 && vRef != 5) {
    Serial.println("Updating analog reference to default");
    analogReference(DEFAULT);
    vRef = 5;
  }
  delay(500);
}
