/*
 * Arduino Pro Micro - Chromebook Setup Automation
 *
 * Acts as a USB HID keyboard to automate Chromebook setup steps.
 * The sequence runs once on power-up (inside setup()), then stops.
 *
 * Onboard LEDs (active LOW):
 *   RX LED : Pin 17  — lit while sequence is running
 *   TX LED : Pin 30  — lit when sequence is complete
 *
 * John Hass <john@getanp.com>
 */

#include <Keyboard.h>

#define RX_LED 17
#define TX_LED 30

// ---------------------------------------------------------------------------
// Delay constants (milliseconds)
// ---------------------------------------------------------------------------
#define DELAY_CHAR      50    // between individual characters
#define DELAY_KEY       150   // after Tab / Enter / special key
#define DELAY_SHORT     500   // short pause between steps
#define DELAY_LONG     3000   // long pause (e.g. waiting for a page to load)
#define DELAY_BOOT     10000   // initial wait before starting (let Chromebook settle)

// ---------------------------------------------------------------------------
// Helper functions
// ---------------------------------------------------------------------------

// Type a single character with a post-delay
void typeChar(char c, int delayMs = DELAY_CHAR) {
  Keyboard.write(c);
  delay(delayMs);
}

// Type a full string, character by character
void typeString(const char* str, int charDelay = DELAY_CHAR) {
  for (int i = 0; str[i] != '\0'; i++) {
    Keyboard.write(str[i]);
    delay(charDelay);
  }
}

// Press Tab
void pressTab(int delayMs = DELAY_KEY) {
  Keyboard.write(KEY_TAB);
  delay(delayMs);
}

// Press Enter
void pressEnter(int delayMs = DELAY_KEY) {
  Keyboard.write(KEY_RETURN);
  delay(delayMs);
}

// Press any special key (KEY_TAB, KEY_RETURN, KEY_UP_ARROW, etc.)
void pressKey(uint8_t key, int delayMs = DELAY_KEY) {
  Keyboard.write(key);
  delay(delayMs);
}

// Hold modifier + key (e.g. Shift+Tab, Ctrl+A)
void pressModKey(uint8_t modifier, uint8_t key, int delayMs = DELAY_KEY) {
  Keyboard.press(modifier);
  Keyboard.press(key);
  delay(50);
  Keyboard.releaseAll();
  delay(delayMs);
}

// Pause between major steps
void shortPause()         { delay(DELAY_SHORT); }
void longPause()          { delay(DELAY_LONG);  }
void delayFor(unsigned long seconds){ delay(seconds * 1000UL); }

// ---------------------------------------------------------------------------
// Setup sequence — edit this to match your Chromebook setup flow
// ---------------------------------------------------------------------------
void runSetup() {
  // Wait for Chromebook to reach the first setup screen
  delayFor(45);
  pressTab();
  delayFor(1);
  pressTab();
  delayFor(1); 
  pressTab();
  delayFor(1);
  pressEnter();
  delayFor(7);
  typeString("<enrollment email>");
  pressEnter();
  delayFor(7);
  typeString("<enrollment password>");
  pressEnter();
  delayFor(60);
  pressEnter();
  delayFor(20);
  pressTab();
  delayFor(1);
  pressTab();
  delayFor(1);
  pressTab();
  delayFor(1);      
  pressEnter();
  delayFor(1);
  pressTab();
  delayFor(1);
  pressTab();
  delayFor(1);
  pressEnter();

}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------
void setup() {
  pinMode(RX_LED, OUTPUT);
  pinMode(TX_LED, OUTPUT);

  // Both LEDs off initially
  digitalWrite(RX_LED, HIGH);
  digitalWrite(TX_LED, HIGH);

  Keyboard.begin();

  // RX LED on while running
  digitalWrite(RX_LED, LOW);

  runSetup();

  Keyboard.end();

  // RX off, fast-blink TX to indicate done
  digitalWrite(RX_LED, HIGH);
}

void loop() {
  digitalWrite(TX_LED, LOW);
  delay(100);
  digitalWrite(TX_LED, HIGH);
  delay(100);
}
