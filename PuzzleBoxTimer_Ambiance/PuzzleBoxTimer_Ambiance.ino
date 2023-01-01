
// Module connection pins (Digital Pins)
#define CLK 8
#define DIO 7
#include <FastLED.h>
#define numberOfSeconds(_time_) ((_time_ / 1000)% 60)
#define numberOfMinutes(_time_) (((_time_ / 1000)/ 60) % 60)
#include <TM1637Display.h>
#define NUM_LEDS 12 //Anzahl LEDs
#define DATA_PIN 4

CRGB leds[NUM_LEDS];
int seconds;
int minutes;
const byte startLED = 5;
const byte startButton = 6;
unsigned long startTime;
int startLEDBlink = 500;

const uint8_t OFF[] = {0, 0, 0, 0};
//                      .GFEDCBA
const uint8_t PLAY[] = {B01110011, B00111000, B01011111, B01101110};
const uint8_t LOST[] = {B00111000, B00111111, B01101101, B01111001};

TM1637Display display(CLK, DIO);

unsigned long timeLimit = 3606000;
unsigned long timeLimitLEDRing = timeLimit;
int Counter = 1;

const byte anzahlSchluessel = 3;
byte schluessel[anzahlSchluessel] = {3, 10, 12};
byte schluesselLED[anzahlSchluessel] = {2, 9, 11};
bool schluesselLastState[anzahlSchluessel] = {1, 1, 1};
bool schluesselCurrentState[anzahlSchluessel] = {1, 1, 1};
byte schluesselAlleSchluessel[anzahlSchluessel] = {0, 0, 0};



#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(A0, A1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(24);  //Set volume value. From 0 to 30
  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  display.setBrightness(0x01);
  display.setSegments(OFF);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(100);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < 7) {
      leds[i] = CRGB::Green;

    }    else if (i < 10) {
      leds[i] = CRGB::Yellow;
    }
    else {
      leds[i] = CRGB::Red;
    }
  }
  FastLED.show();
  startTime = millis();
  displayText();
  for (int i = 0; i < anzahlSchluessel; i++) {
    pinMode(schluessel[i], INPUT_PULLUP);
    pinMode(schluesselLED[i], OUTPUT);
  }

}


void countdown() {
  long timeRemaining = timeLimit - millis();
  while (timeRemaining > 0) {
    if (timeRemaining <= ((timeLimitLEDRing - 2000) - (Counter * ((timeLimitLEDRing - 3000) / 12))) && Counter <= 12) {
      leds[Counter - 1] = CRGB::Black;
      Counter++;
      FastLED.show();
    }
    seconds = numberOfSeconds(timeRemaining);
    minutes = numberOfMinutes(timeRemaining);

    display.showNumberDecEx(seconds, 0, true, 2, 2);
    display.showNumberDecEx(minutes, 0x40, true, 2, 0);

    timeRemaining = timeLimit - millis();
    for (int i = 0; i < anzahlSchluessel; i++) {
      schluesselCurrentState[i] = digitalRead(schluessel[i]);
      if (schluesselCurrentState[i] != schluesselLastState[i]) {
        if (schluesselCurrentState[i] == 0) {
          digitalWrite(schluesselLED[i], HIGH);
        }
        else if (schluesselCurrentState[i] == 1) {
          digitalWrite(schluesselLED[i], LOW);
        }
      }
      schluesselLastState[i] = schluesselCurrentState[i];
    }
    if (memcmp(schluesselCurrentState, schluesselAlleSchluessel, 3) == 0) {
      onSolve();
    }
  }
}
void displayText() {
  display.setSegments(PLAY);
  delay(2000);
}

void lost() {
  myDFPlayer.play(3);
  while (1) {
    fill_solid( leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    display.setSegments(OFF);
    delay(500);
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    display.setSegments(LOST);
    FastLED.show();
    delay(500);
  }
}
void onSolve() {
  myDFPlayer.play(2);
  delay(50);
  myDFPlayer.volume(30);
  while (1) {
    display.setSegments(OFF);
    for (int i = 0; i < anzahlSchluessel; i++) {
      digitalWrite(schluesselLED[i], LOW);
    }
    digitalWrite(startLED, LOW);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(50);
    }


    for (int i = 0; i < anzahlSchluessel; i++) {
      digitalWrite(schluesselLED[i], HIGH);
    }
    digitalWrite(startLED, HIGH);


    display.showNumberDecEx(seconds, 0, true, 2, 2);
    display.showNumberDecEx(minutes, 0x40, true, 2, 0);

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Green;
      FastLED.show();
      delay(50);
    }
  }
}

void loop() {
  if (digitalRead(startButton) == 1) {
    if (millis() > startTime + startLEDBlink) {
      if (digitalRead(startLED) == HIGH) {
        digitalWrite(startLED, LOW);
        startTime = millis();
      }
      else {
        digitalWrite(startLED, HIGH);
        startTime = millis();
      }
    }
  }
  else if (digitalRead(startButton) == 0) {
    digitalWrite(startLED, LOW);
    timeLimit = timeLimit + millis();
    myDFPlayer.play(1);  //Play the first mp3
    displayText();
    countdown();
    lost();
  }
}
