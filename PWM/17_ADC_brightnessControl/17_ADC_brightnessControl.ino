#define LED_PIN 5
#define ADC_PIN A5
#define ADC_RESOLUTION 1024  // 10bit
#define PWM_RESOLUTION 255   // 8bit
#define VREF 5


void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int val = analogRead(ADC_PIN);
  Serial.print("val: ");
  Serial.print(val);

  float volts = map(val, 0, ADC_RESOLUTION - 1, 0, VREF);
  Serial.print(" volts: ");
  Serial.print(volts);


  int dutyCycle = map(val, 0, ADC_RESOLUTION - 1, 0, PWM_RESOLUTION);
  Serial.print(" dutyCycle: ");
  Serial.println(dutyCycle);

  analogWrite(LED_PIN, dutyCycle);
}
