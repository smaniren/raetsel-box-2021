#include <Adafruit_NeoPixel.h>
#define LED_PIN    3 //neopixelpin
#define LED_COUNT 2 // Anzahl LEDs
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); //LED-Strip deklarieren
const byte powerButton = 11;
const byte foerderband1 = 5;
const byte foerderband2 = 6;
int power = 200; //Motor velocity between 0 and 255
bool currentDirection = 0;
bool directionCounter = 0;

const byte foerderbandSwitch = 8;
const int grenzwerte[] = {200, 800};
const byte anzahlPotis = 3;
const byte potiPin[anzahlPotis] = {A5, A4, A3};
const byte anzahlFarben = 5; //Anzahl Levels
const int farben[anzahlFarben][3] = {
  {255, 0, 0},
  {0, 255, 0},
  {0, 255, 255},
  {255, 0, 40},
  {40, 40, 255},
}; //ROT, Grün, Blau
int farbenEingabe[anzahlPotis] = {}; // Hier wird gespeichert, welche farbe gerade gewählt ist
const int sensitivityLED = 20;
byte levelZaehler = 0;
byte anzahlKorrekt = 0;

void setup() {
  /*for (int i = 0; i < anzahlPotis; i++) {
    pinMode(potiPin[i], INPUT);

    }*/
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.begin(9600);

  pinMode(foerderband1, OUTPUT);
  pinMode(foerderband2, OUTPUT);
  pinMode(powerButton, INPUT_PULLUP);
  pinMode(foerderbandSwitch, INPUT_PULLUP);
}

void gewonnen() {
  if (digitalRead(foerderbandSwitch) == LOW) {

    directionCounter = 0;
    if (currentDirection == 0) {
      analogWrite(foerderband1, power);
      analogWrite(foerderband2, 0);
    }
    else {
      analogWrite(foerderband1, 0);
      analogWrite(foerderband2, power);
    }
  }
  else {
    analogWrite(foerderband1, 0);
    analogWrite(foerderband2, 0);
    if (currentDirection == 0 && directionCounter == 0) {
      currentDirection = 1;
      directionCounter = 1;
    }
    else if (currentDirection == 1 && directionCounter == 0) {
      currentDirection = 0;
      directionCounter = 1;
    }
  }
}

void loop() {
  if (digitalRead(powerButton) == 1) {
    /*gewonnen();
      Serial.print(currentDirection);
      Serial.print("     ");
      Serial.println(directionCounter);*/
    for (int i = 0; i < anzahlPotis; i++) {
      farbenEingabe[i] = analogRead(potiPin[i]);
      farbenEingabe[i] = analogRead(potiPin[i]);
      //for (int j = 0; j <= 2; j++) {
      if (farbenEingabe[i] >= grenzwerte[1]) {
        farbenEingabe[i] = 255;
      }
      else if (farbenEingabe[i] >= grenzwerte[0]) {
        farbenEingabe[i] = 40;
      } else {
        farbenEingabe[i] = 0;
      }
      //}
      /*Serial.print(analogRead(potiPin[i]));
        Serial.print("     ");*/

    }
    Serial.print(digitalRead(powerButton));
    Serial.println("");
    if (levelZaehler >= anzahlFarben) {
      for (int i = 0; i < anzahlPotis; i++) {
        pixels.clear();
        pixels.show();
      }

      gewonnen();
    }

    else {
      pixels.setPixelColor(0, pixels.Color(farbenEingabe[0], farbenEingabe[1], farbenEingabe[2]));
      pixels.setPixelColor(1, pixels.Color(farben[levelZaehler][0], farben[levelZaehler][1], farben[levelZaehler][2]));
      pixels.show();
    }
    for (int i = 0; i < anzahlPotis; i++) {

      /*Serial.print(anzahlKorrekt);
        Serial.print("     ");
        Serial.print(farbenEingabe[i]);
        Serial.print("  |   ");
        Serial.print(farben[levelZaehler][i]);
        Serial.print("  ||||   ");
      */
      if (farbenEingabe[i] == farben[levelZaehler][i]) {
        anzahlKorrekt += 1;
      }
      else {
        anzahlKorrekt = 0;
      }
    }
    if (anzahlKorrekt == 3) {
      delay(750);
      for (int i = 0; i < 10 ; i++) {

        pixels.setPixelColor(1, pixels.Color(0, 0, 0));
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
        delay(50);
        pixels.setPixelColor(1, pixels.Color(farben[levelZaehler][0], farben[levelZaehler][1], farben[levelZaehler][2]));
        pixels.setPixelColor(0, pixels.Color(farben[levelZaehler][0], farben[levelZaehler][1], farben[levelZaehler][2]));
        pixels.show();
        delay(50);
      }
      levelZaehler += 1;
      Serial.print("Level ");
      Serial.print(levelZaehler);
      Serial.println(" Bestanden!");
    }
    else {
      anzahlKorrekt = 0;
    }
  }
  else {
    pixels.clear();
    pixels.show();
    analogWrite(foerderband1, 0);
    analogWrite(foerderband2, 0);
  };
}
