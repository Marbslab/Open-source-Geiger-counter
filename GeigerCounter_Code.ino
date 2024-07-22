#include <SPI.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define BUTTON_LEFT 3
#define BUTTON_MIDDLE 4
#define BUTTON_RIGHT 5
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET - 1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);

const byte tubePin = 2;
const byte ledPin = 13;
const long interval_tube = 1000;

// 'sign1', 34x32px
const unsigned char epd_bitmap_sign1 [] PROGMEM = {
	0x00, 0x80, 0x00, 0x40, 0x00, 0x03, 0x80, 0x00, 0x70, 0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 0x0f, 
	0xe0, 0x01, 0xfc, 0x00, 0x0f, 0xe0, 0x01, 0xfe, 0x00, 0x1f, 0xf0, 0x03, 0xfe, 0x00, 0x3f, 0xf0, 
	0x03, 0xff, 0x00, 0x3f, 0xf8, 0x07, 0xff, 0x00, 0x7f, 0xf8, 0x07, 0xff, 0x80, 0x7f, 0xfc, 0x0f, 
	0xff, 0x80, 0x7f, 0xfc, 0x0f, 0xff, 0x80, 0xff, 0xf8, 0xc7, 0xff, 0xc0, 0xff, 0xf9, 0xe7, 0xff, 
	0xc0, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x07, 
	0xf8, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xfe, 
	0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 
	0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00
};
// 'sign2', 30x32px
const unsigned char epd_bitmap_sign2 [] PROGMEM = {
	0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0xff, 0xc0, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xfc, 
	0x20, 0x00, 0xff, 0xf8, 0x78, 0x00, 0xff, 0xf0, 0x7c, 0x00, 0xff, 0xc0, 0x7f, 0x00, 0x3f, 0x00, 
	0xff, 0xc0, 0x1c, 0x00, 0xff, 0xf3, 0xc0, 0x00, 0xff, 0xf7, 0xe0, 0x00, 0xff, 0xe7, 0xe0, 0x00, 
	0xff, 0xe7, 0xe0, 0x00, 0xff, 0xf7, 0xe0, 0x00, 0xff, 0xf3, 0xc0, 0x00, 0xff, 0xc0, 0x1c, 0x00, 
	0x7f, 0x00, 0x3f, 0x00, 0x7c, 0x00, 0xff, 0xc0, 0x78, 0x00, 0xff, 0xf0, 0x20, 0x00, 0xff, 0xf8, 
	0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xe0, 
	0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xf0, 0x00
};

// 'sign3', 34x32px
const unsigned char epd_bitmap_sign3 [] PROGMEM = {
	0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 
	0x7f, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x1f, 
	0xfe, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xf8, 
	0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x00, 0x03, 0xf0, 0x00, 0x00, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0xff, 
	0xf9, 0xe7, 0xff, 0xc0, 0xff, 0xf8, 0x87, 0xff, 0xc0, 0x7f, 0xfc, 0x0f, 0xff, 0x80, 0x7f, 0xfc, 
	0x0f, 0xff, 0x80, 0x7f, 0xf8, 0x07, 0xff, 0x80, 0x3f, 0xf8, 0x07, 0xff, 0x00, 0x3f, 0xf0, 0x03, 
	0xff, 0x00, 0x1f, 0xf0, 0x03, 0xfe, 0x00, 0x1f, 0xe0, 0x01, 0xfc, 0x00, 0x0f, 0xe0, 0x01, 0xfc, 
	0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 0x03, 0x80, 0x00, 0x70, 0x00, 0x00, 0x80, 0x00, 0x40, 0x00
};

// 'sign4', 34x32px
const unsigned char epd_bitmap_sign4 [] PROGMEM = {
	0x00, 0x80, 0x00, 0x40, 0x00, 0x03, 0x80, 0x00, 0x70, 0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 0x0f, 
	0xe0, 0x01, 0xfc, 0x00, 0x0f, 0xe0, 0x01, 0xfe, 0x00, 0x1f, 0xf0, 0x03, 0xfe, 0x00, 0x3f, 0xf0, 
	0x03, 0xff, 0x00, 0x3f, 0xf8, 0x07, 0xff, 0x00, 0x7f, 0xf8, 0x07, 0xff, 0x80, 0x7f, 0xfc, 0x0f, 
	0xff, 0x80, 0x7f, 0xfc, 0x0f, 0xff, 0x80, 0xff, 0xf8, 0xc7, 0xff, 0xc0, 0xff, 0xf9, 0xe7, 0xff, 
	0xc0, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0xff, 0xf3, 0xf3, 0xff, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x00, 
	0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x07, 
	0xf8, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xfe, 
	0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 
	0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00
};

unsigned long previousMillis_tube = 0;
bool OK_state = false; // Needed for the function of the OK button
bool tubeState = LOW; // Boolean variable for the tube output
int clickCounter = 0; // Variable to store the clicks
int minuteIndex = 0; // Loops from 0 to 59 to compute clicks per second
int cps = 0; // Clicks per second
int cpm = 0; // Clicks per minute
int cpmArray[60]; // Array to hold 60 cps values
int screenIndex = 1; // Index which screen is shown

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(tubePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tubePin), clicks, FALLING);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_MIDDLE, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  // Initialize display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);
  // Clear the buffer
  display.clearDisplay();
  display.display();
  // Initialize array
  for (int i = 0; i < 60; i++) {
    cpmArray[i] = 0;
  }
  // Little animation at the beginning
  display.setTextSize(2);
  for(int i = 0; i < 10; i ++) {
    display.drawBitmap(0, 0, epd_bitmap_sign1, 34, 32, 1);
    display.setCursor(36, 0);
    display.print("Geiger");
    display.setCursor(36, 18);
    display.print("counter");
    display.display();
    delay(50);
    // Use filled rectangle instead of display.clearDisplay() to avoid flicker
    display.fillRect(0, 0, 34, 32, BLACK);
    display.display();
    display.drawBitmap(0, 0, epd_bitmap_sign2, 30, 32, 1);
    display.setCursor(36, 0);
    display.print("Geiger");
    display.setCursor(36, 18);
    display.print("counter");
    display.display();
    delay(50);
    display.fillRect(0, 0, 34, 32, BLACK);
    display.display();
    display.drawBitmap(0, 0, epd_bitmap_sign3, 34, 32, 1);
    display.setCursor(36, 0);
    display.print("Geiger");
    display.setCursor(36, 18);
    display.print("counter");
    display.display();
    delay(50);
    display.fillRect(0, 0, 34, 32, BLACK);
    display.display();
    display.drawBitmap(0, 0, epd_bitmap_sign4, 34, 32, 1);
    display.setCursor(36, 0);
    display.print("Geiger");
    display.setCursor(36, 18);
    display.print("counter");
    display.display();
    delay(50);
    display.fillRect(0, 0, 34, 32, BLACK);
    display.display();
  }
}

void loop() {
  while (OK_state == false) {
    if (digitalRead(BUTTON_MIDDLE) == 1) {
      OK_state = !OK_state;
      delay(200);
    }
    if (digitalRead(BUTTON_RIGHT) == 1 && screenIndex < 2) {
      screenIndex++;
      delay(200); // Debounce button
    }
    if (digitalRead(BUTTON_LEFT) == 1 && screenIndex > 0) {
      screenIndex--;
      delay(200);
    }
    if (screenIndex == 0) {
      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(21, 6);
      display.write(0xE6); // µ sign
      display.print("Sv/h");
      display.display();
    }
    if (screenIndex == 1) {
      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(35, 6);
      display.print("CPS");
      display.display();
    }
    if (screenIndex == 2) {
      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(35, 6);
      display.print("CPM");
      display.display();
    }
  }
  if (digitalRead(BUTTON_MIDDLE) == 1) {
    OK_state = !OK_state;
    delay(200);
  }
  tubeState = digitalRead(tubePin);
  if (tubeState == LOW) digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);
  unsigned long currentMillis_tube = millis();
  // If one second is reached...
  if (currentMillis_tube - previousMillis_tube >= interval_tube) {
    previousMillis_tube = currentMillis_tube;
    cps = clickCounter;
    if (screenIndex == 1) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.print("CPS: ");
      display.print(cps);
      display.display();
      Serial.print("CPS: ");
      Serial.println(cps);
    }
    // CPM computation (sliding window)
    cpmArray[minuteIndex] = cps;
    for (int i = 0; i < 60; i++) {
      cpm += cpmArray[i];
    }
    if (screenIndex == 2) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.print("CPM: ");
      display.print(cpm);
      display.display();
    }
    if (screenIndex == 0) {
      // Convert cpm to microsieverts per hour
      float cpmflt = (float) cpm;
      float dose = cpmflt * 0.00812;
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.write(0xE6); // µ sign
      display.print("Sv/h: ");
      display.setCursor(30, 18);
      display.print(dose, 2);
      display.display();
    }
    cpm = 0; // Reset cpm for the next summation of the array values
    minuteIndex++; // Increase minuteIndex every second by 1
    if (minuteIndex > 59) {
      minuteIndex = 0;
    }
    // Reset clickCounter & cps
    clickCounter = 0;
    cps = 0;
  }
}

void clicks() {
  clickCounter++;
}