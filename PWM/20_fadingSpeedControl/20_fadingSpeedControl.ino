
#define ADC_PIN A5
#define ADC_RESOLUTION 1024  // 10bit
#define PWM_RESOLUTION 255   // 8bit
#define VREF 5
#define NUMBER_LEDs 5
#define MAX_DELAY 1000  // max delay (if speed == 0) between loop cycle in microseconds
#define MIN_DELAY 100   // min delay (if speed == 1) between loop cycle in microseconds

typedef enum e_state {
  STATE_ONE,  // even idx LEDs increasing brightness, uneven decreasing
  STATE_TWO   // even idx LEDs decreasing brightness, uneven increasing
} t_state;

t_state g_state = STATE_ONE;

const uint8_t ledPins[5] = { 3, 5, 6, 10, 11 };

void setup() {
  for (uint8_t i = 0; i < 5; ++i) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

// return float in [0; 1]
float getSpeed() {
  uint16_t potVal = analogRead(ADC_PIN);
  return (float)potVal / (ADC_RESOLUTION - 1);
}

void loop() {
  float speed = getSpeed();

  static uint8_t evenBrightness = 0;
  static uint8_t unevenBrightness = PWM_RESOLUTION;
  for (uint8_t i = 0; i < NUMBER_LEDs; ++i) {
    if (i % 2 == 0) {  // even idx
      analogWrite(ledPins[i], evenBrightness);
    } else {  // uneven idx
      analogWrite(ledPins[i], unevenBrightness);
    }
  }

  // update state
  if ((evenBrightness == PWM_RESOLUTION && g_state == STATE_ONE)
      || (evenBrightness == 0 && g_state == STATE_TWO)) {
    g_state = (g_state == STATE_ONE) ? STATE_TWO : STATE_ONE;
  }

  // update brightness
  if (g_state == STATE_ONE) {
    evenBrightness++;
    unevenBrightness--;
  } else {
    evenBrightness--;
    unevenBrightness++;
  }

  delayMicroseconds(MIN_DELAY + (1 - speed) * (MAX_DELAY - MIN_DELAY));
}
