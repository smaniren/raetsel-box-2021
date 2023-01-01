#define DEBUG

#include <DFRobotDFPlayerMini.h>

#include <Arduino.h>

const int numInputs = 15;
const int buttonPins[numInputs] = {2, 3, A1, 5, 6, 7, 8, 9, 10, 11, 12, 22, 14, 15, 23};
//const int buttonPins[numInputs] = {22, 5, 9, 6, 23, 12, A1, 2, 11, 10, 8, 15, 3, 14, 7};
//const int buttonPins[numInputs] = {7, 14, 3, 15 ,8, 10, 11, 2, A1, 12, 23, 6, 9, 5, 22};
const int correctSequence[numInputs] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

DFRobotDFPlayerMini dfPlayer1;
DFRobotDFPlayerMini dfPlayer2;
bool RadioPin = 45;
bool Radiocounter = 0;
bool lastInput[numInputs] = {0};
int currentStep = 0;

#include <FMTX.h>

float fm_freq = 102.7;  // Here set the default FM frequency

void setup() {
  pinMode(RadioPin, INPUT_PULLUP);
  fmtx_init(fm_freq, EUROPE);
  for (int i = 0; i < numInputs; i++) {
    pinMode(buttonPins[i], INPUT);
  }
#ifdef DEBUG
  Serial.begin(115200);
  delay(500);
  Serial.print("FM-Tranmitter Start\r\n");
  Serial.print(F("Initialising software serial interface to DFPlayer1..."));
#endif
  for (int i = 0; i < 10; i++) {
    Serial2.begin(9600);
    delay(500);
    if (dfPlayer1.begin(Serial2)) {
#ifdef DEBUG
      Serial.println(F("OK!"));
#endif
      break;
    }
    else {
      Serial.print(".");
    }
    if (i == 9) {
#ifdef DEBUG
      Serial.println(F("Failed :("));
#endif
      return;
    }
  }
#ifdef DEBUG
  delay(500);
  Serial.print(F("Initialising software serial interface to DFPlayer2..."));
#endif
  for (int i = 0; i < 10; i++) {
    Serial1.begin (9600);
    delay(500);
    if (dfPlayer2.begin(Serial1)) {
#ifdef DEBUG
      Serial.println(F("OK!"));
#endif
      break;
    }
    else {
      Serial.print(".");
    }
    if (i == 9) {
#ifdef DEBUG
      Serial.println(F("Failed :("));
#endif
      return;
    }
  }
  dfPlayer1.volume(30);
  dfPlayer2.volume(30);


#ifdef DEBUG
  Serial.println("Setup Complete");
#endif

  delay(2000);
}


void loop() {
  if(digitalRead(45)== 1 && Radiocounter == 0){
    Radiocounter = 1;
    dfPlayer1.play(1);
    dfPlayer1.enableLoopAll();
}
  for (int i = 0; i < numInputs; i++) {
    int currentReading = digitalRead(buttonPins[i]);
    if (lastInput[i] == LOW && currentReading == HIGH) {
      Serial.print("Taste: ");
      Serial.print(i);
      dfPlayer2.play(i + 1);
      if (correctSequence[currentStep] == i) {
        currentStep++;
#ifdef DEBUG
        /*Serial.print(F("Correct Input! Onto step #"));
          Serial.println (currentStep);*/
#endif
        if (currentStep == numInputs) {
          onSolve();
        }
      }
      else {
        currentStep = 0;
        /*Serial.print(F("Incorrect input! Expected "));
          Serial.print(correctSequence[currentStep]);
          Serial.print(F(" but got "));
          Serial.print(i);*/
      }
    }
    lastInput[i] = currentReading;
  }
}

void onSolve() {
#ifdef DEBUG
  Serial.println(F("Puzzle Solved!"));
#endif
  currentStep = 0;
}
