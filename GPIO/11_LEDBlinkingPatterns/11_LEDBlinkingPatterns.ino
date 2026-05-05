#define BUTTON_PIN 3

#define DEBOUNCE_DELAY 500 // in microseconds

#define PATTERN_CYCLE_TIME 1000           // duration of one sicle in milliseconds

short currentPattern = 1;                 // which pattern is currently being played (1-5)
unsigned long lastPatternStateSwitch = 0; // each pattern alternates between states. Time of the last switch within a pattern in milliseconds
int ledPins[4] = {8, 9, 10, 11};

void setup() {
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);

  Serial.begin(9600);
}

void patternOne() {
  Serial.println("pattern1");

  static short outputValue = HIGH;  // alternates between LOW and HIGH

  digitalWrite(ledPins[0], outputValue);
  digitalWrite(ledPins[1], outputValue);
  digitalWrite(ledPins[2], outputValue);
  digitalWrite(ledPins[3], outputValue);

  outputValue = !outputValue;
}

void patternTwo() {
  Serial.println("pattern2");

  static short outputValue = HIGH;  // alternates between LOW and HIGH

  digitalWrite(ledPins[0], outputValue);
  digitalWrite(ledPins[1], !outputValue);
  digitalWrite(ledPins[2], outputValue);
  digitalWrite(ledPins[3], !outputValue);

  outputValue = !outputValue;
}

void patternThree() {
  Serial.println("pattern3");

  static short patternState = 0;  // alternates between 0 - 3
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);

  digitalWrite(ledPins[patternState], HIGH);

  patternState = (patternState + 1) % 4;
}

void patternFour() {
  Serial.println("pattern4");

  static short outputValue = HIGH; // alternates between HIGH and LOW
                                   //
  digitalWrite(ledPins[0], outputValue);
  digitalWrite(ledPins[1], outputValue);
  digitalWrite(ledPins[2], !outputValue);
  digitalWrite(ledPins[3], !outputValue);

  outputValue = !outputValue;
}

void patternFive() {
  Serial.println("pattern5");

  static short outputValue = HIGH; // alternates between HIGH and LOW
                                   //
  digitalWrite(ledPins[0], !outputValue);
  digitalWrite(ledPins[1], outputValue);
  digitalWrite(ledPins[2], outputValue);
  digitalWrite(ledPins[3], !outputValue);

  outputValue = !outputValue;
}

void updatePattern() {
  switch (currentPattern) {
    case 1:
      patternOne();
      break;
    case 2:
      patternTwo();
      break;
    case 3:
      patternThree();
      break;
    case 4:
      patternFour();
      break;
    case 5:
      patternFive();
      break;
    default:
      break;
  }
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

  if (millis() - lastPatternStateSwitch > PATTERN_CYCLE_TIME / 2) {
    updatePattern();
    lastPatternStateSwitch = millis();
  }
}
