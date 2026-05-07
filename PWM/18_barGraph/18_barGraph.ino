#define ADC_PIN A5
#define ADC_RESOLUTION 1024  // 10bit
#define PWM_RESOLUTION 255   // 8bit
#define VREF 5
#define NUMBER_LEDs 5

const uint8_t ledPins[5] = { 3, 5, 6, 10, 11 };

void setup() {
  // Serial.begin(9600);

  for (uint8_t i = 0; i < 5; ++i) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  uint16_t val = analogRead(ADC_PIN);

  // To divide into 'equal' parts, we chose NUMBER_LEDs as the max limit
  // since this is an invalid index, we adjust for it using the ternary
  uint8_t led = map(val, 0, ADC_RESOLUTION - 1, 0, NUMBER_LEDs);
  led = (led >= NUMBER_LEDs) ? NUMBER_LEDs - 1 : led;

  // if ADC resolution cannot be equally divided, the last range is supposed to be longer
  // to compensate for that (instead of having another really short section)
  // The ternary adjusts for that last section
  uint16_t rangeWidth = ADC_RESOLUTION / NUMBER_LEDs;
  uint8_t dutyCycle = map(val % rangeWidth, 0, rangeWidth - 1, 0, PWM_RESOLUTION);
  dutyCycle = (val / rangeWidth >= NUMBER_LEDs) ? 255 : dutyCycle;

  // Serial.print("val: ");
  // Serial.print(val);
  // Serial.print(" led: ");
  // Serial.print(led);
  // Serial.print(" dutyCycle: ");
  // Serial.println(dutyCycle);

  for (int i = 0; i < led; ++i) {
    analogWrite(ledPins[i], PWM_RESOLUTION);
  }
  analogWrite(ledPins[led], dutyCycle);
  delay(10);
}
