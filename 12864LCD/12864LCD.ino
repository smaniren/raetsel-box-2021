#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Arduino.h>



const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2',},
  {'3', '4', '5',},
  {'6', '7', '8',}
};
int LED[9] = {8, 9, 12, A0, A1, A2, A3, A4, A5};
int LEDState[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte rowPins[ROWS] = {7, 6, 5}; //connect to the row pinouts of the keypad SPALTEN
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad ZEILEN

int anzahlTaster = 9;
int korrekt = 0;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R1, 13, 11, 10, A7);

int arrTaster[9][2] = {
  {0, 0}, {1, 0}, {2, 0},
  {0, 1}, {1, 1}, {2, 1},
  {0, 2}, {1, 2}, {2, 2}
};
byte code1[9] {1, 0, 1, 0, 1, 0, 1, 0, 1};
int key;
unsigned long timeStart;
int countdown = 5000;
int level[] = {10000, 7000, 4500};
int levelCount = 0;
int anzahlLevels = 3;
byte RadioCounter = 0;

int arrLoesung[15][2] = {
  {0, 0}, {1, 0}, {2, 0},
  {0, 1}, {1, 1}, {2, 1},
  {0, 2}, {1, 2}, {2, 2},
  {0, 3}, {1, 3}, {2, 3},
  {0, 4}, {1, 4}, {2, 4}
};

byte Loesungarray[15] = {
  0,0,1,
  0,1,0,
  1,0,1,
  0,0,0,
  0,1,1
};

int arrLoesungNr[15] = {
  0, 0, 5,
  0, 4, 0,
  6, 0, 2,
  0, 0, 0,
  0, 1, 3
};

int arrLoesungNrPos[15][2] = {
  {2, 32}, {24, 32}, {51, 32},
  {2, 55}, {28, 55}, {47, 55},
  {5, 78}, {24, 78}, {51, 78},
  {2, 101}, {24, 101}, {51, 101},
  {2, 124}, {28, 124}, {51, 124}
};

void setup(void) {
  pinMode(1, OUTPUT);
  u8g2.begin();
  u8g2.setFont(u8g2_font_logisoso16_tf);
  //Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
}

void loop(void) {
  while (levelCount < anzahlLevels) {
    countdown = level[levelCount];
    if (timeStart <= millis()) {
      timeStart = millis() + countdown;
      for (size_t i = 0; i < anzahlTaster - 1; i++)
      {
        size_t j = random(0, anzahlTaster - i);
        int t = code1[i];
        code1[i] = code1[j];
        code1[j] = t;
      }
      for (int i = 0; i < anzahlTaster; i++) {
        digitalWrite(LED[i], LOW);
        LEDState[i] = 0;
      }

    }

    key = customKeypad.getKey() - 48;
    if (key != -48) {
      if (LEDState[key] == 1) {
        digitalWrite(LED[key], LOW);
        LEDState[key] = 0;
      }
      else {
        digitalWrite(LED[key], HIGH);
        LEDState[key] = 1;
      }
    }


    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 16);
      u8g2.print("Level ");
      u8g2.print(levelCount + 1);
      u8g2.setCursor(0, 42);
      u8g2.print("Zeit: ");

      u8g2.drawBox(0, 47, map((timeStart - millis()), 0, countdown, 0, 64), 11);
      for (int i = 0; i < anzahlTaster; i++) {
        if (code1[i] == 0) {
          u8g2.drawDisc((8 + (arrTaster[i][0] * 23)), (73 + (arrTaster[i][1] * 23)), 8);
        }
        else {
          u8g2.drawCircle((8 + (arrTaster[i][0] * 23)), (73 + (arrTaster[i][1] * 23)), 8);
        }
      }
      //Verbindungslinien Zeichnen
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
          u8g2.drawHLine((17 + (i * 23)), (73 + (j * 23)), 6);
          u8g2.drawVLine((8 + (j * 23)), (82 + (i * 23)), 6);
        }
      }
    } while ( u8g2.nextPage() );

    for (int i = 0; i < anzahlTaster; i++) {
      if (code1[i] == LEDState[i]) {
        korrekt += 1;
      }
      else {
        korrekt = 0;
      }
    }

    if (korrekt >= 9) {
      for (int i = 0; i < anzahlTaster; i++) {
        digitalWrite(LED[i], LOW);
        LEDState[i] = 0;
      }
      korrekt = 0;
      levelCount += 1;
      timeStart = 0;
      u8g2.clear();
    }
  }
  u8g2.firstPage();
  do {
    //Pfeil zeichnen
    u8g2.drawTriangle(25, 0, 25, 18, 10, 9);
    u8g2.drawBox(25, 6, 25, 6);   
    //Zahlen zeichnen
      u8g2.setFont(u8g2_font_7x13_tf);
      for (int i = 0; i < 15; i++) {
        u8g2.setCursor(arrLoesungNrPos[i][0], arrLoesungNrPos[i][1]);
        u8g2.print(arrLoesungNr[i]);
      }
    //Kreise zeichnen
    for (int i = 0; i < 15; i++) {
      if (Loesungarray[i] == 0) {
        u8g2.drawDisc((8 + (arrLoesung[i][0] * 23)), (27 + (arrLoesung[i][1] * 23)), 8);
      }
      else {
        u8g2.drawCircle((8 + (arrLoesung[i][0] * 23)), (27 + (arrLoesung[i][1] * 23)), 8);
       }
      }
    } while ( u8g2.nextPage());
  //Daten an Arduino Mega senden
  if(RadioCounter==0){
  digitalWrite(1, HIGH);
  RadioCounter = 1;
  }
  }
