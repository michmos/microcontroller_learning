#define PIN_BUTTON 8
#define LONG_PRESS_DURATION 1000000 // the time that a click counts as a long press in microseconds
#define DOUBLE_PRESS_BREAK 300000   // the break between two presses so that it counts as a double press in microseconds

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

bool  isPressed = false;
bool  longPressDetected = false;
bool  clickDetected = false;
bool  doubleClickDetected = false;
unsigned long eventTime = 0;

void loop() {

  int buttonState = digitalRead(PIN_BUTTON);
  delayMicroseconds(500);
  if (digitalRead(PIN_BUTTON) != buttonState) {
    // bouncing -> ignore
    return;
  }

  unsigned long currentTime = micros();
  unsigned long duration = currentTime - eventTime;

  if ((buttonState == LOW && isPressed) || (buttonState == HIGH && !isPressed)) {
    // state didn't change
    if (isPressed && !longPressDetected && !doubleClickDetected && duration >= LONG_PRESS_DURATION) {
      Serial.println("Long press detected");
      longPressDetected = true;
    } else if (!isPressed 
        && !clickDetected && !doubleClickDetected && !longPressDetected
        && duration > DOUBLE_PRESS_BREAK) {
      Serial.println("Single click detected");
      clickDetected = true;
    }
    return;
  }

  eventTime = currentTime;

  if (buttonState == LOW) {   // button pressed
    clickDetected = false;
    doubleClickDetected = false;
    longPressDetected = false;
    if (duration <= DOUBLE_PRESS_BREAK) {
      Serial.println("Double click detected");
      doubleClickDetected = true;
    }
    isPressed = true;
  } else {                    // button released
    isPressed = false;
  }
}
