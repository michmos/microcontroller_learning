#define LED_PIN 5
#define PWM_RESOLUTION 255
#define PWM_FREQUENCY 980

const float CYCLE_TIME = 1.0 / PWM_FREQUENCY;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // increase brighntess
  unsigned long startTime = millis();
  for (int dutyCycle = 0; dutyCycle <= PWM_RESOLUTION; ++dutyCycle) {
    analogWrite(LED_PIN, dutyCycle);
    delayMicroseconds(2000000 / 255);
  }
  Serial.print("Increase: ");
  Serial.println(millis() - startTime);

  startTime = millis();
  // decrease brighntess
  for (int dutyCycle = PWM_RESOLUTION; dutyCycle >= 0; --dutyCycle) {
    analogWrite(LED_PIN, dutyCycle);
    delayMicroseconds(1000000 / 255);
  }
  Serial.print("Decrease: ");
  Serial.println(millis() - startTime);
}
