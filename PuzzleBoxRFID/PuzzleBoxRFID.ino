// \xE1 = ä | \xF5 = ü | \xEF = ö
/*byte nummern[10][7] =
  { {0x04, 0xE6, 0x15, 0xc2, 0x6E, 0x67, 0x80}, //0
  {0x04, 0xEB, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //1
  {0x04, 0xEF, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //2
  {0x04, 0xE2, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //3
  {0x04, 0xF4, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //4
  {0x04, 0xF8, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //5
  {0x04, 0x10, 0x15, 0xC2, 0x6E, 0x67, 0x81}, //6
  {0x04, 0xFC, 0x15, 0xC2, 0x6E, 0x67, 0x80}, //7
  {0x04, 0x00, 0x15, 0xC2, 0x6E, 0x67, 0x81}, //8
  {0x04, 0x04, 0x15, 0xC2, 0x6E, 0x67, 0x81}  //9
  };*/
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
Servo myservo;
const byte servo = 10;
const byte numReaders = 2;
const byte sdaPins[numReaders] = {A0, A1};
const byte resetPin = 9;
const byte signalPin = A2;
const int servoOpen = 90;
const int servoClosed = 30;

String LCDtext[5][2] = {
  {" a  m  e Au     ", "b   dr   W\xF5r "},
  {" ie  ie     na  ", " n  ch    hr    "},
  {"  =  *x         ", "  =   |   = ?   "},
  {"  za   Fe  er   ", "be     h  h     "},
  {" ag   r    hr   ", " u ch  \xF5n    "},
};

byte loesungen[5][2][7] = {
  {{0x04, 0xEB, 0x15, 0xC2, 0x6E, 0x67, 0x80}, {0x04, 0x00, 0x15, 0xC2, 0x6E, 0x67, 0x81}},//18
  {{0x04, 0x04, 0x15, 0xC2, 0x6E, 0x67, 0x81}, {0x04, 0x10, 0x15, 0xC2, 0x6E, 0x67, 0x81}},//96
  {{0x04, 0xEB, 0x15, 0xC2, 0x6E, 0x67, 0x80}, {0x04, 0xEF, 0x15, 0xC2, 0x6E, 0x67, 0x80}},//12
  {{0x04, 0x10, 0x15, 0xC2, 0x6E, 0x67, 0x81}, {0x04, 0xF4, 0x15, 0xC2, 0x6E, 0x67, 0x80}},//64
  {{0x04, 0xFC, 0x15, 0xC2, 0x6E, 0x67, 0x80}, {0x04, 0xE2, 0x15, 0xC2, 0x6E, 0x67, 0x80}},//73
};
byte reset[4] = {0x49, 0x07, 0xC9, 0xB9};
int level = 0;
bool resetBool = 0;
MFRC522 mfrc522[numReaders];

#include <LiquidCrystal.h>
int lcdContrastPin = 5;
int lcdContrast = 115;
LiquidCrystal lcd(6, 7, 3, 4, 2, 8); //(RS, E, D4, D5, D6, D7)



void setup() {
  myservo.attach(servo);
  myservo.write(servoClosed);
  pinMode(signalPin, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  lcd.begin(16, 2);
  analogWrite(lcdContrastPin, lcdContrast);
  for (int i = 0; i < numReaders; i++) {
    mfrc522[i].PCD_Init(sdaPins[i], resetPin);
    Serial.print(F("Reader #"));
    Serial.print(i);
    Serial.print(F(" Initialized on pin "));
    Serial.print(String(sdaPins[i]));
    Serial.print(F(". Antenna strenght: "));
    Serial.print(mfrc522[i].PCD_GetAntennaGain());
    Serial.print(F(". Version: "));
    mfrc522[i].PCD_DumpVersionToSerial();
    if (mfrc522[i].PCD_PerformSelfTest() == 1) {
      Serial.println(F("Passed Self-Test"));
      mfrc522[i].PICC_HaltA();
      mfrc522[i].PCD_Init();
    };
    delay(500);
  }
  lcd.setCursor(0, 0);
  lcd.print(LCDtext[level][0]);

  lcd.setCursor(0, 1);
  lcd.print(LCDtext[level][1]);
  Serial.println(F("--- END SETUP ---"));
}

void loop() {
  for (int i = 0; i < numReaders; i++) {
    mfrc522[i].PCD_Init();
    String readRFID = "";

    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {

      ///////////////////Um IDs auszulesen!////////////////////////////
      /*Serial.print("UID #");
        Serial.print(i);
        Serial.print(":");
        for (byte j = 0; j < mfrc522[i].uid.size; j++) {
        Serial.print(mfrc522[i].uid.uidByte[j] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522[i].uid.uidByte[j], HEX);
        }*/
      ////////////////////////////////////////////////////////////////


      if (memcmp(mfrc522[1].uid.uidByte, reset, 4) == 0) {
        if (resetBool == 0) {
          myservo.write(servoClosed);
          resetBool = 1;
  Serial.println("zu");
          
        }
        else if (resetBool == 1) {
          myservo.write(servoOpen);
          resetBool = 0;
  Serial.println("auf");
        }
          digitalWrite(signalPin, HIGH);
          delay (1000);
          digitalWrite(signalPin, LOW);
      }
      if ((memcmp(mfrc522[1].uid.uidByte, loesungen[level][0], 7) == 0) && (memcmp(mfrc522[0].uid.uidByte, loesungen[level][1], 7) == 0)) {
        korrekt();
      }
    }
    mfrc522[i].PICC_HaltA();
    mfrc522[i].PCD_StopCrypto1();
  }
}

void korrekt() {
  Serial.print("Level #");
  Serial.print(level + 1);
  Serial.println(" bestanden!");
  level++;
  if (level > 4) {
    fertig();
  }
  digitalWrite(signalPin, HIGH);
  delay (2000);
  digitalWrite(signalPin, LOW);
  lcd.setCursor(0, 0);
  lcd.print(LCDtext[level][0]);
  lcd.setCursor(0, 1);
  lcd.print(LCDtext[level][1]);
}
void fertig() {

  Serial.print("--- Alle Level bestanden ---");
  for (int i = 0; i < 40; i++) {
    digitalWrite(signalPin, HIGH);
    delay (20);
    digitalWrite(signalPin, LOW);
    delay (20);
  }
  myservo.write(servoOpen);
  lcd.setCursor(0, 0);
  lcd.print("  Sehr gut,     ");
  lcd.setCursor(0, 1);
  lcd.print("  Weiter so!    ");
  for (int i; i < 2; i++) {
    mfrc522[i].PICC_HaltA();
    mfrc522[i].PCD_StopCrypto1();
  }

  while (1) {
  }
}
