//KABEL---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DEBUGKabel
const int kabelButton = A5;
const byte anzahlBuchsen = 8;
const byte signalPins[anzahlBuchsen] = {7, 8, 9, 10, A0, A1, A2, A3};
const byte numVerbindungen = 4;
const byte verbindungen[numVerbindungen][2] = {{0, 7}, {1, 4}, {2, 6}, {3, 5}};
bool letzterStatus[numVerbindungen] = {false, false, false, false};
enum PuzzleStatus {Initialisierung, Betrieb, Fertig};
PuzzleStatus puzzleStatus = Initialisierung;
byte Fertigcounter = 0;
int LEDKabel = A4;
const byte dcPin[2] = {11, 5};
const byte stopPin = 12;
bool KabelFertig = 0;
const byte stopPinOpen = 3;//Braun zu Blau von arduino aus
const byte stopPinClosed = 4;//Gelb zu Grün von Arduino aus

void setup() {

  pinMode(stopPinOpen, INPUT_PULLUP);
  pinMode(stopPinClosed, INPUT_PULLUP);
  for (int i = 0; i < anzahlBuchsen; i++) {
    pinMode(signalPins[i], INPUT_PULLUP);
  }
  pinMode(LEDKabel, OUTPUT);
  pinMode(kabelButton, INPUT);
#ifdef DEBUGKabel
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
#endif
  digitalWrite(LEDKabel, HIGH);
  for (int i = 0; i < 1; i++) {
    pinMode(dcPin[i], OUTPUT);
  }


}

void loop() {
  Serial.print(digitalRead(stopPinClosed));
  Serial.print(digitalRead(stopPinOpen));
  if (puzzleStatus == Initialisierung) {
#ifdef DEBUGKabel
    Serial.println(F("Initialisierung"));
#endif
    if (digitalRead(stopPinClosed) == 1) {
      analogWrite(dcPin[0], 255);
      analogWrite(dcPin[1], 0);
    }
    else if (digitalRead(stopPinClosed) == 0) {
      analogWrite(dcPin[0], 0);
      analogWrite(dcPin[1], 0);
      puzzleStatus = Betrieb;
      Serial.print("Betrieb");
    }
  }
  if (puzzleStatus == Fertig) {
    digitalWrite(LEDKabel, LOW);
    if (Fertigcounter == 0) {
      for (int i = 0; i < 10; i++) {
        digitalWrite(LEDKabel, HIGH);
        delay(50);
        digitalWrite(LEDKabel, LOW);
        delay(50);
      }
      Fertigcounter = 1;
    }

    if (digitalRead(stopPinOpen) == 1) {
      analogWrite(dcPin[0], 0);
      analogWrite(dcPin[1], 255);
    }
    else if (digitalRead(stopPinOpen) == 0) {
      analogWrite(dcPin[0], 0);
      analogWrite(dcPin[1], 0);
      while (1) {}
    }
  }
  else {
    if (digitalRead(kabelButton) == HIGH && puzzleStatus != Fertig) {
      digitalWrite(LEDKabel, HIGH);
      bool alleKabelKorrekt = true;

      bool statusVeraendert = false;

      for (int i = 0; i < numVerbindungen; i++) {
        byte pin1 = signalPins[verbindungen[i][0]];
        byte pin2 = signalPins[verbindungen[i][1]];
        bool jetztStatus = istVerbunden(pin1, pin2);
        if (jetztStatus != letzterStatus[i]) {
          statusVeraendert = true;
          letzterStatus[i] = jetztStatus;
        }

        if (jetztStatus == false) {
          alleKabelKorrekt = false;
        }
      }
      if (statusVeraendert) {
#ifdef DEBUGKabel
        for (uint8_t i = 0; i < numVerbindungen; i++) {
          Serial.print(F("Pin#"));
          Serial.print(signalPins[verbindungen[i][0]]);
          Serial.print(F(" - Pin#"));
          Serial.print(signalPins[verbindungen[i][1]]);
          Serial.print(F(" : "));
          Serial.println(letzterStatus[i] ? "CONNECTED" : "NOT CONNECTED");
        }
        Serial.println(F("---"));
#endif
      }
      /*if (timeRunning == 0 && puzzleStatus == Fertig) {
        timeBegin = millis();
        timeRunning = 1;
        }
        if (millis() > timeBegin + 100) {
        if (digitalRead(LEDKabel) == HIGH) {
          digitalWrite(LEDKabel, LOW);
        }
        else {
          digitalWrite(LEDKabel, HIGH);
        }
        timeRunning = 0;
        }*/
      if (alleKabelKorrekt && puzzleStatus == Betrieb) {
        puzzleStatus = Fertig;
#ifdef DEBUGKabel
        Serial.print(F("The puzzle has been solved!"));
#endif
        digitalWrite(LEDKabel, LOW);
      }
    }
    else {
      digitalWrite(LEDKabel, LOW);
    }
  }
}


bool istVerbunden(byte OutputPin, byte InputPin) {
  pinMode(OutputPin, OUTPUT);
  pinMode(InputPin, INPUT_PULLUP);
  delay(50);
  digitalWrite(OutputPin, LOW);

  bool istVerbunden = !digitalRead(InputPin);
  pinMode(OutputPin, INPUT_PULLUP);
  return istVerbunden;
}
