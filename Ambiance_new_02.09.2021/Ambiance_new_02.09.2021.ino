
#include <Arduino.h>

//Libraries für den Mp3-Player
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
SoftwareSerial mySoftwareSerial(A0, A1); // RX, TX --> Initialisieren des Softwareports
DFRobotDFPlayerMini myDFPlayer;          // Initialisieren des DF-Players


// Variables will change:
int ledState = LOW;                   // ledState used to set the LED
String displayText;                   // Text, der auf dem Display angezeigt wird

unsigned long timeLimit = 3606000;    // Dauer des Spiels 3606000
unsigned long previousMillis = 0;     // will store last time LED was updated
const long interval = 500;           // interval at which to blink (milliseconds)

//variablen um zu prüfen, ob das Display geupdatet werden muss oder nicht:
int seconds = 0;
int lastSecond = 99;
int minutes;
int hours;
int progress;

//zuweisung der pins des grünen Start-Buttons:
const byte startButton = 6;
const byte startLED = 5;

//
const byte anzahlSchluessel = 3;
byte schluessel[anzahlSchluessel] = {7, 10, 12};
bool schluesselLastState[anzahlSchluessel] = {1, 1, 1};
bool schluesselCurrentState[anzahlSchluessel] = {1, 1, 1};
byte schluesselAlleSchluessel[anzahlSchluessel] = {0, 0, 0};


void setup() {
  //setzen der PinModes des grünen Startbuttons
  pinMode(startButton, INPUT_PULLUP);
  pinMode(startLED, OUTPUT);

  //setzen der PinModes der drei Schlüssel:
  for (int i = 0; i < anzahlSchluessel; i++) {
    pinMode(schluessel[i], INPUT_PULLUP);
  }

  //Starten der beiden Serial-Ports:
  Serial.begin(19200);
  mySoftwareSerial.begin(9600);
  delay(2000);
  //Starten des DF-Players:
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  myDFPlayer.volume(24);  //Set volume value. From 0 to 30
}


void loop() {
  if (digitalRead(startButton) == 1) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
        displayText = "PLAY";
      } else {
        ledState = LOW;
        displayText = "";
      }
      digitalWrite(startLED, ledState);
      sendText("t0", displayText);
    }
  }
  else if (digitalRead(startButton) == 0) {
    digitalWrite(startLED, LOW); // damit die LED sicher aus ist.
    myDFPlayer.play(1);  //Play the first mp3
    countdown();
  }
}
void countdown() {
  Serial.print("page 1"); //Pagenummer (links neben Bezeichnung)
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  timeLimit = timeLimit + millis();
  while (1) {
    unsigned long timeRemaining = timeLimit - millis();
    if (timeRemaining > 0) {
      unsigned long delta = timeRemaining;
      hours = delta / 3600000;
      delta = delta - (hours * 3600000);
      minutes = delta / 60000;
      delta = delta - (minutes * 60000);
      seconds = delta / 1000;
      progress = 100 / float(timeLimit) * float(timeRemaining);
      if (seconds != lastSecond) {
        //Sekunden angeben:
        lastSecond = seconds;
        sendNumbers("t1", hours);
        sendNumbers("t2", minutes);
        sendNumbers("t3", seconds);

        //Progressbar updaten
        sendProgress("j0", progress);
        //Progressbar einfärben
        if (progress < 10) {
          sendColor("j0", "63488");
        }
        else if (progress < 30) {
          sendColor("j0", "65504");
        }
        else {
          sendColor("j0", "1024");
        }
      }
      for (int i = 0; i < anzahlSchluessel; i++) {
        schluesselCurrentState[i] = digitalRead(schluessel[i]);
        if (schluesselCurrentState[i] != schluesselLastState[i]) {
          if (schluesselCurrentState[i] == 0) {
            sendPic("p" + String(i), "1");
          }
          else if (schluesselCurrentState[i] == 1) {
            sendPic("p" + String(i), "0");
          }
        }
        schluesselLastState[i] = schluesselCurrentState[i];
      }
      if (memcmp(schluesselCurrentState, schluesselAlleSchluessel, 3) == 0) {
        onSolve();
      }
    }
    else {
      onLost();
    }
  }
}



void sendNumbers(String field, int data) {
  Serial.print(field);
  Serial.print(".txt=");
  Serial.print("\"");
  if (data < 10) {
    Serial.print("0");
  }
  Serial.print(String(data));
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
void sendProgress(String field, int data) {
  Serial.print(field);
  Serial.print(".val=");
  Serial.print(data);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
void sendText(String field, String data) {
  Serial.print(field);
  Serial.print(".txt=");
  Serial.print("\"");
  Serial.print(data);
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
void sendColor(String field, String color) {
  Serial.print(field);
  Serial.print(".pco=");
  Serial.print(color);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
void sendPic(String field, String picId) {
  Serial.print(field);
  Serial.print(".pic=");
  Serial.print(picId);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void     onLost() {
  myDFPlayer.play(3);
  sendProgress("j0", 0);
  Serial.print("page 2"); //Pagenummer (links neben Bezeichnung)
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(500);
  while (1) {
    sendText("t0", "");
    sendText("t1", "");
    sendText("t2", "");
    sendText("t3", "");
    sendProgress("j0", 0);
    delay(500);
    sendText("t0", "Time's up!");
    sendText("t1", "00");
    sendText("t2", "00");
    sendText("t3", "00");
    sendProgress("j0", 100);
    delay(500);
  }
}

void onSolve() {
  myDFPlayer.play(2);
  delay(50);
  myDFPlayer.volume(30);
  Serial.print("page 3"); //Pagenummer (links neben Bezeichnung)
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  sendProgress("j0", progress);
  sendNumbers("t1", hours);
  sendNumbers("t2", minutes);
  sendNumbers("t3", seconds);
  delay(500);
  while (1) {
    sendText("t0", "");
    sendProgress("j0", 0);
    delay(500);
    sendText("t0", "You WIN!");
    sendProgress("j0", progress);
    delay(500);
  }
}
