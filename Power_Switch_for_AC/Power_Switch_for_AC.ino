/*  Duda.AC.v1.1

  This is a program to control a GE window air conditioning unit utlizing the built in pushbuttons 
  (connections scratched on the board to disconnect).  It will control power, compressor, and fan.
  Eventually it may integrate with thermostat.  For now it's ON or OFF.
  
  NOTE:  Add sounds to each action

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/

#include "pitches.h"
// constants won't change. They're used here to set pin numbers:
const int powerButton = 1; 
const int modeButton = 3;  // can choose the fan speed
const int lowSpeed = 4;  //  Turns the fan on low
const int medSpeed = 5;  //  Turns the fan on medium
const int highSpeed = 6;  //  Turns the fan on high
const int speakerPin = 7;  //  The piezo buzzer is connected to this
const int compressorPin = 8;  //  This pin is connected to the relay the controls the compressor
const int thermostatPin = 9;  //To be used in the future when I read the thermostate via serial.

int shaveMelody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 }; //Shave and a haircut melody
int shaveDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 }; // Shave and a Haircut note length
int zippityDuda[] = { NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_A5, NOTE_F4, NOTE_F4, NOTE_D4, NOTE_E4, NOTE_E4 };  //  Zippity Duda melody
int zippityDurations[] = { 4, 8, 8, 4, 2, 4, 8, 8, 4, 2 }; //  Zippity Duda note length

// Variables will change:
int stateCompressorPin = LOW;
int statePowerButton = LOW;
int previous = LOW;
long time = 0;
long debounce = 200;
int modeButtonCounter = 0;   // counter for the number of button presses
int modeState = 0;         // current state of the button
int lastModeState = 0;     // previous state of the button

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.

void setup() {
  pinMode(powerButton, INPUT);
  pinMode(modeButton, INPUT);
  pinMode(lowSpeed, OUTPUT);
  pinMode(medSpeed, OUTPUT);
  pinMode(highSpeed, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(compressorPin, OUTPUT);
  pinMode(thermostatPin, INPUT);
  digitalWrite(compressorPin, LOW);  //  Makes sure the compressor is off when the AC is plugged in
  digitalWrite(lowSpeed, LOW);       //  Makes sure the fan is off when the AC is plugged in
  digitalWrite(medSpeed, LOW);      //  Makes sure the fan is off when the AC is plugged in
  digitalWrite(highSpeed, LOW);     //  Makes sure the fan is off when the AC is plugged in
  
// the following plays a startup melody (in this case Shave and a Haircut).
  {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int shaveDuration = 1000 / shaveDurations[thisNote];
    tone(8, shaveMelody[thisNote], shaveDuration);
    int pauseBetweenNotes = shaveDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
  Serial.begin(9600);
}

void loop() {

//  This section of the code is to turn the unit on and off at the push of the powerButton

statePowerButton = digitalRead(powerButton);  
  if(statePowerButton == HIGH && previous == LOW && millis() - time > debounce) {
    if(statePowerButton == HIGH){
      stateCompressorPin = LOW; 
      digitalWrite(compressorPin, HIGH);  //Turns the compressor ON
      digitalWrite(lowSpeed, HIGH);
      digitalWrite(medSpeed, LOW);
      digitalWrite(highSpeed, LOW);
    }
                                                                                                          
    else {
       stateCompressorPin = HIGH; 
       digitalWrite(compressorPin, LOW);  // Turns the compressor OFF
       digitalWrite(lowSpeed, LOW);  //  Turns of the fan
       digitalWrite(medSpeed, LOW);  //  Turns off the fan
       digitalWrite(highSpeed, LOW);  /// Turns off the fan
    }
    time = millis();
  }
  digitalWrite(compressorPin, stateCompressorPin);
  previous == stateCompressorPin;

  // this section changes the fan speed when the mode button is pushed

  modeState = digitalRead(modeButton);
  if (modeState != lastModeState) {
    if (modeState == HIGH) {
      modeButtonCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(modeButtonCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastModeState = modeState;

   if (modeButtonCounter == 1) {
    digitalWrite(lowSpeed, HIGH);  //  Turns the fan on low speed
    digitalWrite(medSpeed, LOW);
    digitalWrite(highSpeed, LOW);
    tone(7, 440, 4); // plays a tone when the fan is low
  }
  else if (modeButtonCounter == 2) {
    digitalWrite(lowSpeed, LOW);
    digitalWrite(medSpeed, HIGH);  //  Turns the fan on medium speed
    digitalWrite(highSpeed, LOW);
    tone(7, 600, 4); // plays a tone when the fan is medium
  }
  else if (modeButtonCounter == 3) {
    digitalWrite(lowSpeed, LOW);
    digitalWrite(medSpeed, LOW);
    digitalWrite(highSpeed, HIGH);  //  Turns the fan on high speed
     tone(7, 800, 4); // plays a tone when the fan is high
  }
  else if (modeButtonCounter == 4) {
    modeButtonCounter = 0;
  }
}
  
  
