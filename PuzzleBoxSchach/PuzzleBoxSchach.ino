#define DEBUG
const byte numPieces = 5;
const int sensitivity = 20;
const byte inputPins[] = {A0, A1, A2, A3, A4};
int baseReadings[numPieces];
int currentReadings[numPieces];
enum PuzzleState {Initializing, Running, Solved};
PuzzleState puzzleState = Initializing;

const byte numVmeters = 4;
byte vmeterPins[] = {6,9,10,11};
int vmeterValues[] = {255,132,102,180};

void onSolve() {
  //HIER V-METER
  //puzzleState = Solved;
  for (int i = 0; i < numVmeters; i++) {
    analogWrite(vmeterPins[i],vmeterValues[i]);
  }
  delay(2000);
  for (int i = 0; i < numVmeters; i++) {
    analogWrite(vmeterPins[i],0);
  }
  delay(500);
}

void onUnsolve() {
#ifdef DEBUG
  Serial.print("The puzzle is no longer solved");
#endif
  puzzleState = Running;
  //V-Meter Stop
}

void setup(){
#ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Setup");
#endif
  for (int i = 0; i < numPieces; i++) {
    pinMode(inputPins[i], INPUT);
  }
  delay(500);
  for (int i = 0; i < numVmeters; i++) {
    pinMode(vmeterPins[i], OUTPUT);
  }
  
}

void reset() {
  int numSamples = 15;
  for (int i = 0; i < numPieces; i++) {
    long total = 0;
    for (int j = 0; j < numSamples; j++) {
      delay(100);
      total += analogRead(inputPins[i]);
    }
    baseReadings[i] = total / numSamples;
#ifdef DEBUG
    Serial.println("Reset");
    Serial.print("Sensor #");
    Serial.print(i);
    Serial.print(" Initialized at base level: ");
    Serial.println(baseReadings[i]);
    Serial.println("---");
#endif
  }
}


void loop() {
  
    //onSolve();
  if (puzzleState == Initializing) {
    delay(500);
    reset();
    puzzleState = Running;
  }
  bool allPiecesCorrect = true;
  for (int i = 0; i < numPieces; i++) {
    currentReadings[i] = analogRead(inputPins[i]);
    if ((currentReadings[i] - baseReadings[i]) < sensitivity) {
      allPiecesCorrect = false;
    }
    delay(100);
#ifdef DEBUG
    Serial.print(currentReadings[i]);
    Serial.print(" (");
    Serial.print(currentReadings[i] - baseReadings[i]);
    Serial.print(")    |    ");
#endif
  }
#ifdef DEBUG
  Serial.println("");
#endif
  if (puzzleState == Running && allPiecesCorrect) {
    onSolve();
  }
  if (puzzleState == Solved && !allPiecesCorrect) {
    onUnsolve();
  }
  delay(100);
}
