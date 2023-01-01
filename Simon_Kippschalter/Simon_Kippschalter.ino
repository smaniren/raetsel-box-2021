//KIPP-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const byte kippAnzahlInputs = 2;
const byte kippPins[kippAnzahlInputs] = {A4, A5};
byte kippPinsState[kippAnzahlInputs];
bool printZaehler = 0;
bool printAllowed = 0;
#include "Adafruit_Thermal.h"

// Here's the syntax when using SoftwareSerial (e.g. Arduino Uno) --------
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 2 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 3 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor


//SIMON-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define DEBUGsimon
const byte simonAnzahlButtons = 4;
const byte simonInputPins[simonAnzahlButtons] = {A0, A1, A2, A3};
const byte simonAnzahlSchritte = 6;
const byte schritte[simonAnzahlSchritte] = {3, 0, 0, 1, 2, 0};
const byte simonZahlen = 4;
const bool simonZahl1 [simonZahlen][simonAnzahlSchritte] = {{HIGH, HIGH, HIGH, LOW, LOW, LOW}, {HIGH, LOW, LOW, LOW, LOW, LOW}, {HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, {HIGH, HIGH, LOW, LOW, LOW, LOW}};
const byte simonLEDPins[simonAnzahlSchritte] = {4, 5, 6, 7, 8, 9};
bool simonLetzerStatus[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int simonMomStatus = 0;
unsigned long letzterDruckZeit = 0;
unsigned long druckDelay = 50;
bool simonFertig = 0;
int anzahlBlinken = 30;
bool simonTimerBool = 0;
unsigned long simonTimerLED = 0;
bool simonLEDBool = 0;
byte arrayZaehler = 0;
int Dauer = 1000;
bool endeBool = 0;
int SimonFertigcounter = 0;

void setup() {
  //KIPP-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < kippAnzahlInputs; i++) {
    pinMode(kippPins[i], INPUT_PULLUP);
  }
  mySerial.begin(9600);  // Initialize SoftwareSerial
  //Serial1.begin(19200); // Use this instead if using hardware serial
  printer.begin();        // Init printer (same regardless of serial type)
  printer.boldOn();
  printer.justify('C');
  //Simon--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < simonAnzahlButtons; i++) {
    pinMode(simonInputPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < simonAnzahlSchritte; i++) {
    pinMode(simonLEDPins[i], OUTPUT);
  }
#ifdef DEBUGsimon
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
#endif
}

void loop() {
  //KIPP-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if ((digitalRead(kippPins[0]) == HIGH || digitalRead(kippPins[1]) == HIGH) && printZaehler == 1) {
    printZaehler = 0;
    printAllowed = 0;
  }

  if (digitalRead(kippPins[0]) == LOW && digitalRead(kippPins[1]) == LOW && printZaehler == 0) {
    printZaehler = 1;
    printAllowed = 1;
  }
//  if (printAllowed == 1) {
//    printer.wake();
//    delay(500);
//    printAllowed = 0;
//    printer.println(F("VNMUPJEINNVUKUASU3A8PQMUMK7NM"));
//    printer.println(F("GF2B73OJ3JJCKNI6QF6U4HAGDRDVFP62"));
//    printer.println(F("QJ2LGAUCBUO277EFE67EGOGPKRNIQHLC"));
//    printer.println(F("UMUSKOAG4DVR5SOMDJNFCLR24QF3RFFO"));
//    printer.println(F("THKD7FB4OIQMDM612KHB81EDJA7RCE2N"));
//    printer.println(F("O8LN4DOBJLLEOVU6:4N3SA6E844EAAJK"));
//    printer.println(F("AKBO75TUE4BPVJR5MEPR4VAD1AC13HK,"));
//    printer.println(F("RDVGHOPNHFE1F1548P8IJ3V1,7MGIKH7"));
//    printer.println(F("V1OF2322C1TA7IC6IN2Q4D5G8VCFOCUC"));
//    printer.println(F("8N1HA3JPFHUQF8NLU3AGQ4O6K8EPLQPV"));
//    printer.println(F("GG17CVRFVG4M2SFG4MGLG7SIO2CQG51S"));
//    printer.println(F("K23TBD6I37H8VA4S4RFGBRJADQ2TKFGQ"));
//    printer.println(F("7E1,PF7B1QTMHA1JBN67STNFFKECPF5G"));
//    printer.println(F(""));
//    printer.println(F("--> ! VORSICHTIG ABREISSEN ! <--"));
//    printer.feed(5);
//    //printer.setDefault(); // Restore printer to defaults
//  }

  if (printAllowed == 1) {
    printer.wake();
    delay(500);
    printAllowed = 0;
    printer.println(F("VN   J I   U U  U      U  7"));
    printer.println(F("G  B  O  f  K  6   U4 A  R  F  2"));
    printer.println(F("Q   G U B O   E  6 E O   RNI  LC"));
    printer.println(F("   S   G  VR5  D JN  2A    3    "));
    printer.println(F(" TH   KD          DJ  A7  R CE2N"));
    printer.println(F("O       JLL  VU : 4 EA   A  J  K"));
    printer.println(F("  B  5T              V   AC 3  ,"));
    printer.println(F("R  G  PN  E  1 4   I 3  ,  G    "));
    printer.println(F("        V1  OF   2            32"));
    printer.println(F("8    3   H  F       4O6K   Q  PV"));
    printer.println(F("GG  17      2  G4MG  7  O2   1  "));
    printer.println(F("K    2        3          FGHG  T"));
    printer.println(F("7  ,   B  TMH     N67S   KEC  5 "));
    printer.println(F(""));
    printer.println(F("--> ! VORSICHTIG ABREISSEN ! <--"));
    printer.feed(5);
    //printer.setDefault(); // Restore printer to defaults
  }

  //SIMON--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (simonFertig != 0) {
    simonFertig1();
  }
  if ((millis() - letzterDruckZeit) > druckDelay) {
    for (int i = 0; i < simonAnzahlButtons; i++) {
      int jetztInputStatus = digitalRead(simonInputPins[i]);
      if (jetztInputStatus != simonLetzerStatus[i]) {
        letzterDruckZeit = millis();
      }
      if (jetztInputStatus == LOW && simonLetzerStatus[i] == HIGH) {
        if (schritte[simonMomStatus] == i) {
          simonMomStatus++;
#ifdef DEBUGsimon
          Serial.print(F("Correct input! Onto step #"));
          Serial.println(simonMomStatus);
#endif
        }
        else {
          simonMomStatus = 0;
#ifdef DEBUGsimon
          Serial.println(F("Incorrect input! Back to Start!"));
#endif
        }
      }
      simonLetzerStatus[i] = jetztInputStatus;
    }
  }
  if (simonMomStatus == simonAnzahlSchritte) {
    simonFertig1();
#ifdef DEBUGsimon
    Serial.print(F("Puzzle solved!"));
#endif
  }
  else if (simonFertig != 0) {
  }
  if (simonFertig == 0) {
    for (int i = 0; i < simonAnzahlSchritte; i++) {
      digitalWrite(simonLEDPins[i], (i < simonMomStatus ? HIGH : LOW));
    }
  }
}



void simonFertig1() {
  simonFertig = 1;
  if (anzahlBlinken > 0) {
    if (simonTimerBool == 0) {
      simonTimerBool = 1;
      simonTimerLED = millis();
    }
    if (millis() > simonTimerLED + 50) {
      if (simonLEDBool == 0) {
        for (int j = 0; j < simonAnzahlSchritte; j++) {
          digitalWrite(simonLEDPins[j], HIGH);
          simonLEDBool = 1;
        }
      }
      else {
        for (int j = 0; j < simonAnzahlSchritte; j++) {
          digitalWrite(simonLEDPins[j], LOW);
          simonLEDBool = 0;
        }
      }
      simonTimerBool = 0;
      anzahlBlinken -= 1;
    }
  }
  else if (anzahlBlinken == 0) {
    if (endeBool == 0) {
      endeBool = 1;
      simonTimerLED = millis();
    }
    if (arrayZaehler <= simonZahlen - 1) {
      if (millis() > simonTimerLED + Dauer) {
        for (int i = 0; i < simonAnzahlSchritte; i++) {
          digitalWrite(simonLEDPins[i], simonZahl1[arrayZaehler][i]);
        }
      }
      if (millis() > simonTimerLED + Dauer * 2) {
        for (int i = 0; i < simonAnzahlSchritte; i++) {
          digitalWrite(simonLEDPins[i], LOW);
          simonTimerLED = millis();
        }
        if (arrayZaehler < 3) {
          arrayZaehler += 1;
        }
        else {
          if (SimonFertigcounter <4) {
            simonTimerLED = millis() + 1000;
            arrayZaehler = 0;
            SimonFertigcounter += 1;
          }
          else {
            SimonFertigcounter = 0;
            simonMomStatus = 0;
            letzterDruckZeit = 0;
            druckDelay = 50;
            simonFertig = 0;
            anzahlBlinken = 30;
            simonTimerBool = 0;
            simonTimerLED = 0;
            simonLEDBool = 0;
            arrayZaehler = 0;
            Dauer = 1000;
            endeBool = 0;
            simonTimerLED = millis() + 1000;
            arrayZaehler = 0;
          }
        }
      }
    }
  }
}
