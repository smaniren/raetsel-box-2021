#include <Keypad.h>
#include <pitches.h>
#include <Servo.h>
Servo myservo; 
/*<<------------------------------------------------------------------------------------------------------------------------->>*/
/*<<---*/             const byte anzahlZiffern = 7; /*HIER ANZHAHL ZIFFERN EINGEBEN!                                     ---->>*/
/*<<---*/             char loesung[anzahlZiffern] = {'5', '3', '7', '0', '5', '6','#'}; /*HIER PASSWORT EINGEBEN!        ---->>*/
/*<<------------------------------------------------------------------------------------------------------------------------->>*/
const byte buzzer = 3;
const byte gruenLED = A0;
const byte rotLED = A1;
const byte servo = 5;
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 12}; //connect to the column pinouts of the keypad
char eingabe[anzahlZiffern];
byte zaehler = 0;
byte loesungZaehler = 0;

const int servoOpen = 185;
const int servoClosed = 125;
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(rotLED, OUTPUT);
  pinMode(gruenLED, OUTPUT);
  myservo.attach(servo);
  myservo.write(servoClosed);
}

void loop() {
  char customKey = customKeypad.getKey();

  if (zaehler >= anzahlZiffern) {  if ((memcmp(loesung, eingabe, anzahlZiffern) * anzahlZiffern) == 0) {
    //Right
    Serial.println("CORRECT!");
    gewonnen();
  } else {
    //wrong
    Serial.println("WRONG!");
    falsch();
  }
  }

  if (customKey) {

    eingabe[zaehler] = customKey;
    Serial.print("Eingabe #");
    Serial.print(zaehler);
    Serial.print(" = ");
    Serial.print(eingabe[zaehler]);
    Serial.println(" ");
    zaehler++;
    tone(buzzer, 900);
    delay(20);
    if (customKey == '#' && zaehler <anzahlZiffern){
      falsch();
    }
  }
  else {
    noTone(buzzer);
  }
}


void falsch() {
  myservo.write(servoClosed);
  digitalWrite(rotLED, HIGH);
  tone(buzzer, 266,100);
  delay(200);
  tone(buzzer, 200);
  delay(800);
  noTone(buzzer);
  digitalWrite(rotLED, LOW);
  loesungZaehler = 0;
  zaehler = 0;
}
void gewonnen() {
  digitalWrite(gruenLED, HIGH);
  tone(buzzer, 197,100);
  delay(200);
  tone(buzzer, 222,100);
  delay(200);
  tone(buzzer, 250,100);
  delay(200);
  tone(buzzer, 266);
  delay(800);
  noTone(buzzer);
  digitalWrite(gruenLED, LOW);
  zaehler = 0;
  myservo.write(servoOpen);
}
