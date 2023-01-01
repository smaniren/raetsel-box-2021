#include <FastLED.h>

#include <ArduinoJson.h>
#include <LedControl.h>
#include "font.h"

const byte startButton = 8;
const byte buttonPins[] = {2, 3, 4, 5};
const byte buttonPinsLED = 9;
const byte submitPin = 6;
const byte submitPinLED = 7;
const byte codeLength = 4;
const byte numSymbols = 10;
const byte numLeds = 16;
//LedControl lc=LedControl(DIN (braun),CLK(orange),CS (rot),ANZAHLDISPLAYS) 456
//uno
LedControl lc = LedControl(11, 13, 10, codeLength);
//nano
//LedControl lc = LedControl(4, 6, 5, numberOfDisplays);

bool lastButtonState[codeLength];
byte secret[codeLength] = {7, 4, 9, 7};
byte guess[codeLength];
int numGuesses = 0;
CRGB leds[numLeds];
/*
void setRandomSecret() {
  randomSeed(analogRead(A5));
  for (int i = 0; i < codeLength; i++) {
    secret[i] = random(numSymbols);
  }
}*/

void displayResponse(int numBlacks, int numWhites) {
  if ((numBlacks + numWhites) * (numLeds / 4)<1){
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Red;}
    FastLED.show();
    delay(200);
  }
  for (int loop = 0; loop < 7; loop ++) {
    for (int i = 0; i < numBlacks * (numLeds / 4); i++) {
      leds[i] = CRGB::Green;
    }
    for (int i = numBlacks * (numLeds / 4); i < (numBlacks + numWhites) * (numLeds / 4); i++) {
      leds[i] = (loop % 2) ?  CRGB::Black : CRGB::Orange;
    }
    FastLED.show();
    delay(200);
  }
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}


/*
  void displayResponse(int numBlacks, int numWhites){
  for (int loop=0; loop<7;loop++){
    for(int i=0;i<numBlacks*(numLeds/4);i++){
      leds[i] = CRGB::Green;
    }
    for(int i = numBlacks*(numLeds/4); i<(numBlacks+numWhites)*(numLeds/4);i++){
      leds[i] = (loop%2) ? CRGB::Black:CRGB::Orange;
    }
    FastLED.show();
    delay(200);
  }
  for(int i=0; i<numLeds;i++){
  leds[i]=CRGB::Black;
  }
  FastLED.show();
  }
*/

bool serialUpdate (byte secret[], byte guess[], byte blackCount, byte whiteCount) {
  StaticJsonDocument<256> doc;
  doc["id"] = "Mastermind";
  doc["turn"] = numGuesses;
  JsonArray jsonSecret = doc.createNestedArray("secret");
  for (int i = 0; i < codeLength; i++) {
    jsonSecret.add(secret[i]);
  }
  JsonArray jsonGuess = doc.createNestedArray("guess");
  for (int i = 0; i < codeLength; i++) {
    jsonGuess.add(guess[i]);
  }
  JsonArray result = doc.createNestedArray("result");
  result.add(blackCount);
  result.add(whiteCount);
  doc["state"] = (blackCount == codeLength) ? "SOLVED" : "UNSOLVED";
  serializeJson(doc, Serial);
  Serial.println("");
}




bool calculateResponse(byte &blackCount, byte &whiteCount) {
  bool secretUsed[codeLength] = {false, };
  bool guessUsed[codeLength] = {false, };
  blackCount = 0;
  for (int i = 0; i < codeLength; i++) {
    if (guess[i] == secret[i]) {
      blackCount++;
      secretUsed[i] = true;
      guessUsed[i] = true;
    }
  }
  whiteCount = 0;
  for (int i = 0; i < codeLength; i++) {
    if (!guessUsed[i]) {
      for (int j = 0; j < codeLength; j++) {
        if (!secretUsed[j] && (guess[i] == secret[j])) {
          whiteCount++;
          secretUsed[j] = true;
          break;
        }
      }
    }
  }
  serialUpdate(secret, guess, blackCount, whiteCount);
  return (blackCount == codeLength);


}



void getInput() {
  //For testing
  /*
    int counter =0;
    while(!Serial.available());
    while(true){
    char incomingByte = Serial.read();
    if (incomingByte == '\n' ||incomingByte == '\r' ){
      break;
      }
    }
    if(incomingByte==-1){continue;}
    guess[counter]=(incomingByte-48);
    counter++;
    if(counter >= codeLength){break;}
    }*/

  while (!digitalRead(submitPin)) {
    delay(100);
  }

  while (digitalRead(submitPin)) {
    for (int i = 0; i < codeLength; i++) {
      bool buttonState = digitalRead(buttonPins[i]);
      if (!buttonState && lastButtonState[i]) {
        guess[i]++;
        if (guess[i] >= numSymbols) {
          guess[i] = 0;
        }
      }
      lastButtonState[i] = buttonState;
      for (int row = 0; row < 8; row++) {
        lc.setRow(i, row, digits[guess[i]][row]);
      }
    }
  }
}




void setup() {

  for (int i = 0; i < codeLength; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 2);
    lc.clearDisplay(i);
  }
pinMode(startButton,INPUT_PULLUP);
pinMode(submitPinLED, OUTPUT);
pinMode(submitPinLED, OUTPUT);

  for (int i = 0; i < codeLength; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(submitPin, INPUT_PULLUP);

  Serial.begin(9600);
  FastLED.addLeds<WS2812B, A0, GRB>(leds, numLeds);
//setRandomSecret();
}


void loop() {
  getInput();
  byte whiteCount = 0, blackCount = 0;
  calculateResponse(blackCount, whiteCount);

  if (blackCount == codeLength) {
    Serial.print(F("YOU WIN!"));

    for (int i = 0; i < numLeds; i++) {
      leds[i] = CRGB::Green;
      FastLED.show();
      delay(20);
    }
    for (int i = 0; i < numLeds; i++) {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(20);
    }
  }
  else {
    displayResponse(blackCount, whiteCount);
  }
  numGuesses++;
}
