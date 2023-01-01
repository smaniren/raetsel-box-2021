//MORS--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <pitches.h>
#include <AnalogSmooth.h>
const byte morsButton = A2;
const byte morsSwitch[2] = {A0, A1};
int morsSwitchState[2] = {0, 0};
int morsSwitchLastState[2] = {0, 0};
const byte morsLED = 13;
unsigned long LEDStart;
const int morsBuzzer = 11;
byte morsEingabe = 0;
//const int morsPoti = A7;
int morsPotiVal;
AnalogSmooth as = AnalogSmooth(5);
int Tempo = 300;
//int potiTempoAuswahl[2] = {400, 200};
byte code1[8] = {4, 3, 1, 1, 1, 3, 1, 1};//X --> C
byte code2[8] = {4, 3, 1, 3, 1, 1, 1, 3};//Viereck --> Q
byte code3[8] = {4, 3, 1, 1, 1, 1, 1, 3};//C --> X

//-- Lösung auf SWchloss somit 3610
byte codeAuswahl[8] = {0, 0, 0, 0, 0, 0, 0, 0};
byte codePosition = 0;

void setup() {
  //MORS--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  pinMode(morsLED, OUTPUT);
  pinMode(morsButton, INPUT_PULLUP);
  pinMode(morsSwitch[0], INPUT_PULLUP);
  pinMode(morsSwitch[1], INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {

  //MORS--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
STARTMORS:
  if (digitalRead(morsButton) == HIGH) {
    for (int i = 0; i < 2; i++) {
      morsSwitchState[i] = digitalRead(morsSwitch[i]);
      if (morsSwitchState[i] != morsSwitchLastState[i]) {
        morsSwitchLastState[i] = morsSwitchState[i];
        digitalWrite(morsLED, LOW);
        noTone(morsBuzzer);
        codePosition = 0;
        goto STARTMORS;
      }
    }
    if (digitalRead(morsSwitch[0]) == HIGH && digitalRead(morsSwitch[1]) == HIGH) {
      for (int i = 0; i < 8; i++) {
        codeAuswahl[i] = code1[i];
      }
    } else if (digitalRead(morsSwitch[0]) == LOW && digitalRead(morsSwitch[1]) == HIGH) {
      for (int i = 0; i < 8; i++) {
        codeAuswahl[i] = code2[i];
      }

    } else if (digitalRead(morsSwitch[0]) == HIGH && digitalRead(morsSwitch[1]) == LOW)  {
      for (int i = 0; i < 8; i++) {
        codeAuswahl[i] = code3[i];
      }
    }
    if (codePosition > 7) {
      codePosition = 0;
    }
    if (morsEingabe == 0) {
      morsEingabe = 1;
      LEDStart = millis();
    }
    if (millis() > LEDStart + codeAuswahl[codePosition]*Tempo && digitalRead(morsLED) == 0) {
      digitalWrite(morsLED, HIGH);
      tone(morsBuzzer, NOTE_F3);
      morsEingabe = 0;
      codePosition += 1;
    }
    else if (millis() > LEDStart + codeAuswahl[codePosition]*Tempo && digitalRead(morsLED) == 1) {
      digitalWrite(morsLED, LOW);
      noTone(morsBuzzer);
      morsEingabe = 0;
      codePosition += 1;
    }
  }
  else {
    digitalWrite(morsLED, LOW);
    noTone(morsBuzzer);
    codePosition = 0;
  }

}
