/*************************************************
 * Public Constants
 *************************************************/
 
 #define NOTE_B0  31
 #define NOTE_C1  33
 #define NOTE_CS1 35
 #define NOTE_D1  37
 #define NOTE_DS1 39
 #define NOTE_E1  41
 #define NOTE_F1  44
 #define NOTE_FS1 46
 #define NOTE_G1  49
 #define NOTE_GS1 52
 #define NOTE_A1  55
 #define NOTE_AS1 58
 #define NOTE_B1  62
 #define NOTE_C2  65
 #define NOTE_CS2 69
 #define NOTE_D2  73
 #define NOTE_DS2 78
 #define NOTE_E2  82
 #define NOTE_F2  87
 #define NOTE_FS2 93
 #define NOTE_G2  98
 #define NOTE_GS2 104
 #define NOTE_A2  110
 #define NOTE_AS2 117
 #define NOTE_B2  123
 #define NOTE_C3  131
 #define NOTE_CS3 139
 #define NOTE_D3  147
 #define NOTE_DS3 156
 #define NOTE_E3  165
 #define NOTE_F3  175
 #define NOTE_FS3 185
 #define NOTE_G3  196
 #define NOTE_GS3 208
 #define NOTE_A3  220
 #define NOTE_AS3 233
 #define NOTE_B3  247
 #define NOTE_C4  262
 #define NOTE_CS4 277
 #define NOTE_D4  294
 #define NOTE_DS4 311
 #define NOTE_E4  330
 #define NOTE_F4  349
 #define NOTE_FS4 370
 #define NOTE_G4  392
 #define NOTE_GS4 415
 #define NOTE_A4  440
 #define NOTE_AS4 466
 #define NOTE_B4  494
 #define NOTE_C5  523
 #define NOTE_CS5 554
 #define NOTE_D5  587
 #define NOTE_DS5 622
 #define NOTE_E5  659
 #define NOTE_F5  698
 #define NOTE_FS5 740
 #define NOTE_G5  784
 #define NOTE_GS5 831
 #define NOTE_A5  880
 #define NOTE_AS5 932
 #define NOTE_B5  988
 #define NOTE_C6  1047
 #define NOTE_CS6 1109
 #define NOTE_D6  1175
 #define NOTE_DS6 1245
 #define NOTE_E6  1319
 #define NOTE_F6  1397
 #define NOTE_FS6 1480
 #define NOTE_G6  1568
 #define NOTE_GS6 1661
 #define NOTE_A6  1760
 #define NOTE_AS6 1865
 #define NOTE_B6  1976
 #define NOTE_C7  2093
 #define NOTE_CS7 2217
 #define NOTE_D7  2349
 #define NOTE_DS7 2489
 #define NOTE_E7  2637
 #define NOTE_F7  2794
 #define NOTE_FS7 2960
 #define NOTE_G7  3136
 #define NOTE_GS7 3322
 #define NOTE_A7  3520
 #define NOTE_AS7 3729
 #define NOTE_B7  3951
 #define NOTE_C8  4186
 #define NOTE_CS8 4435
 #define NOTE_D8  4699
 #define NOTE_DS8 4978

 // notes in the melody:
int melody[] = {
  NOTE_D4, NOTE_F3, NOTE_B3, NOTE_F3,      // D4-F3-B3-F3
  NOTE_D4, NOTE_GS3, NOTE_B3, NOTE_GS3,    // D4-Ab3-B3-Ab3
  NOTE_G4, NOTE_B3, NOTE_E4, NOTE_B3,      // G4-B3-E4-B3
  NOTE_E4, NOTE_A3, NOTE_CS4, NOTE_A3,     // E4-A3-C#4-A3
  NOTE_D4, NOTE_FS4, NOTE_B3, NOTE_FS4,    // D4-F#4-B3-F#4
  NOTE_D4, NOTE_GS3, NOTE_B3, NOTE_GS3,    // D4-Ab3-B3-Ab3
  NOTE_G4, NOTE_B3, NOTE_E4, NOTE_B3,      // G4-B3-E4-B3
  NOTE_E4, NOTE_A3, NOTE_CS4, NOTE_A3,     // E4-A3-C#-A3
};
 
int outputPin     = 5;
int maxChords     = 8;
int maxRepeats    = 4;         // every chord will be repeated 4 times
int noteDuration  = 1000/8;    // 4 = quarter note, 8 = eight note, etc.


const int LEDPin0 = 6; // LED left
const int LEDPin1 = 7; // LED center
const int LEDPin2 = 8; // LED right

const int analogOutPin2 = 9; // Motor
const int analogOutPin1 = 10; // Motor

void setup() {
  Serial.begin(9600); // Inicializar la comunicación serial a 9600 baudios
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(LEDPin0, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
}

void dance_leds(){
  digitalWrite(LEDPin0,LOW);
  digitalWrite(LEDPin1,LOW);
  digitalWrite(LEDPin2,LOW);
  delay(100);
  digitalWrite(LEDPin0,HIGH);
  digitalWrite(LEDPin1,LOW);
  digitalWrite(LEDPin2,LOW);
  delay(100);
  digitalWrite(LEDPin0,HIGH);
  digitalWrite(LEDPin1,HIGH);
  digitalWrite(LEDPin2,LOW);
  delay(100);
  digitalWrite(LEDPin0,HIGH);
  digitalWrite(LEDPin1,HIGH);
  digitalWrite(LEDPin2,HIGH);
  delay(100);
  digitalWrite(LEDPin0,LOW);
  digitalWrite(LEDPin1,HIGH);
  digitalWrite(LEDPin2,HIGH);
  delay(100);
  digitalWrite(LEDPin0,LOW);
  digitalWrite(LEDPin1,LOW);
  digitalWrite(LEDPin2,HIGH);
  delay(100);
  digitalWrite(LEDPin0,LOW);
  digitalWrite(LEDPin1,LOW);
  digitalWrite(LEDPin2,LOW);
  delay(100);
}

void playSound() {
  // play all 8 chords
  for (int chords = 0; chords < maxChords; chords++) {
    // play every chord 4 times
    for (int chordTone = 0; chordTone < 4; chordTone++) {
      // play all 4 tones of every chord
      for (int repeats = 0; repeats < maxRepeats; repeats++) {
        // play the tone
        tone(outputPin, melody[chords*maxRepeats + repeats], noteDuration);
        // set a delay between the notes
        delay(noteDuration);
        // stop the tone playing
        noTone(outputPin);
      }
    }
  }
}

void loop() {
  // Leer datos del ESP32
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    Serial.print("Recieved Data: ");
    Serial.println(data);

    if (strcmp(data.c_str(), "SMALL FORWARD") == 0){
      Serial.println("Order: FORWARD");
      analogWrite(analogOutPin1, 50);
      analogWrite(analogOutPin2, 50);
      digitalWrite(LEDPin0,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin2,HIGH);
      }

    else if (strcmp(data.c_str(), "FORWARD") == 0){
      Serial.println("Order: FORWARD");
      analogWrite(analogOutPin1, 100);
      analogWrite(analogOutPin2, 100);
      digitalWrite(LEDPin0,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin2,HIGH);
      }

    else if (strcmp(data.c_str(), "TURBO FORWARD") == 0){
      Serial.println("Order: FORWARD");
      analogWrite(analogOutPin1, 160);
      analogWrite(analogOutPin2, 160);
      digitalWrite(LEDPin0,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin2,HIGH);
      }

    else if (strcmp(data.c_str(), "LEFT") == 0){
      Serial.println("Order: LEFT");
      analogWrite(analogOutPin1, 160);
      analogWrite(analogOutPin2, 30);
      digitalWrite(LEDPin0,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin2,HIGH);
      }
    else if (strcmp(data.c_str(), "RIGHT") == 0){
      Serial.println("Order: RIGHT");
      analogWrite(analogOutPin1, 30);
      analogWrite(analogOutPin2, 160);
      digitalWrite(LEDPin0,HIGH);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin2,LOW);
      }
     else if (strcmp(data.c_str(), "STOP") == 0){
      Serial.println("Order: STOP");
      analogWrite(analogOutPin1, 0);
      analogWrite(analogOutPin2, 0);
      digitalWrite(LEDPin0,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin2,LOW);
      }
     else if (strcmp(data.c_str(), "DANCE") == 0){
      Serial.println("Order: DANCE");
      analogWrite(analogOutPin1, 40);
      analogWrite(analogOutPin2, 40);
      digitalWrite(LEDPin0,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin2,HIGH);
      playSound();
      dance_leds();
      analogWrite(analogOutPin1, 0);
      analogWrite(analogOutPin2, 0);
      }
  }
  delay(100);
}
