#define BUTTON_PIN 3
#define DEBOUNCE_DELAY 500                // in microseconds
#define PATTERN_STATE_DURATION 500        // duration of each state in a pattern in milliseconds

short currentPattern = 0;                 // which pattern is currently being played (1-5)
int ledPins[4] = {8, 9, 10, 11};

// [pattern][patternState][ledPin]
int patterns[5][4][4] = {
  // pattern 1
  {
    {HIGH, HIGH, HIGH, HIGH},
    {LOW, LOW, LOW, LOW},
    {HIGH, HIGH, HIGH, HIGH},
    {LOW, LOW, LOW, LOW}
  },
  // pattern 2
  {
    {HIGH, LOW, HIGH, LOW},
    {LOW, HIGH, LOW, HIGH},
    {HIGH, LOW, HIGH, LOW},
    {LOW, HIGH, LOW, HIGH}
  },
  // pattern 3
  {
    {HIGH, LOW, LOW, LOW},
    {LOW, HIGH, LOW, LOW},
    {LOW, LOW, HIGH, LOW},
    {LOW, LOW, LOW, HIGH}
  },
  // pattern 4
  {
    {HIGH, HIGH, LOW, LOW},
    {LOW, LOW, HIGH, HIGH},
    {HIGH, HIGH, LOW, LOW},
    {LOW, LOW, HIGH, HIGH}
  },
  // pattern 5
  {
    {LOW, HIGH, HIGH, LOW},
    {HIGH, LOW, LOW, HIGH},
    {LOW, HIGH, HIGH, LOW},
    {HIGH, LOW, LOW ,HIGH}
  }
};


void setup() {
  for (int i = 0; i < 4; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (int i = 0; i < 4; ++i) {
    digitalWrite(ledPins[i], LOW);
  }

  Serial.begin(9600);
}


void updatePattern() {
  static short patternState = 0;
  for(int i = 0; i < 4; ++i) {
    digitalWrite(ledPins[i], patterns[currentPattern][patternState][i]);
  }
  patternState = (patternState + 1) % 4;
}

void processButtonPin() {
  static int buttonState = HIGH;
  static bool isPressed = false;
  static unsigned long lastEventTime = 0;

  int newButtonState = digitalRead(BUTTON_PIN);
  int currentTime = micros();

  if (newButtonState != buttonState && currentTime - lastEventTime > DEBOUNCE_DELAY) {
    if (!isPressed && newButtonState == LOW) {
      currentPattern = (currentPattern % 5) + 1;
    }
    isPressed = !isPressed;
    lastEventTime = currentTime;
    buttonState = newButtonState;
  }
}

void loop() {
  processButtonPin();

  /* each pattern consists of two or more states
   * we switch between those states every PATTERN_STATE_DURATION ms
   */
  static unsigned long lastPatternStateSwitchTime = 0;
  if (millis() - lastPatternStateSwitchTime > PATTERN_STATE_DURATION) {
    updatePattern();
    lastPatternStateSwitchTime = millis();
  }
}
