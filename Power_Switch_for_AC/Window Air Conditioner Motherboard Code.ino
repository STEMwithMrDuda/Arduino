/*  Duda.AC.v3.0

  This is a program to control a GE window air conditioning unit utlizing the built in pushbuttons
  (connections scratched on the board to disconnect).  It will control power, compressor, and fan.
  Eventually it may integrate with thermostat.  For now it's ON or OFF.

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

// variables will change:
int stateCompressorPin = LOW;     // the current state of LED
int stateFanPin = LOW;  //  Is the fan on or off?
int lastButtonState;    // the previous state of button
int statePowerButton; // the current state of button
long time = 0;
long debounce = 200;
int modeButtonCounter = 0;   // counter for the number of button presses
int powerState = LOW;
int newModeCount;

void setup() {

  pinMode(powerButton, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(compressorPin, OUTPUT);          // set arduino pin to output mode
  pinMode(lowSpeed, OUTPUT);
  pinMode(medSpeed, OUTPUT);
  pinMode(highSpeed, OUTPUT);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(thermostatPin, INPUT);
  statePowerButton = LOW;

  // plays music
  {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int shaveDuration = 1000 / shaveDurations[thisNote];
      tone(8, shaveMelody[thisNote], shaveDuration);
      int pauseBetweenNotes = shaveDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(7);
    }
  }
}
void loop() {
  lastButtonState = statePowerButton;  // save the last state
  statePowerButton = digitalRead(powerButton);  // read new state

  // This section checks to see if the powerButton is pressed.  If so it turns the fan on LOW and turns on the compressor.
  if (lastButtonState == HIGH && statePowerButton == LOW)
  {
    stateCompressorPin = !stateCompressorPin;
    powerState = !powerState;
    digitalWrite(medSpeed, LOW);
    digitalWrite(highSpeed, LOW);
    digitalWrite(compressorPin, stateCompressorPin);
    digitalWrite(lowSpeed, stateCompressorPin);
    digitalWrite(powerState, powerState);
  }

  //  This section controls the speed of the fan ONLY if powerState is HIGH
  if (powerState == HIGH) {
    if (digitalRead(modeButton) == LOW && stateCompressorPin == HIGH) // Need to add something here to make sure the AC unit is on.
    { newModeCount = modeButtonCounter + 1;
      if (newModeCount != modeButtonCounter) {
        Serial.println(newModeCount);
        switch (newModeCount) {
          case 1: digitalWrite(lowSpeed, HIGH), digitalWrite(medSpeed, LOW), digitalWrite(highSpeed, LOW); break;
          case 2: digitalWrite(lowSpeed, LOW), digitalWrite(medSpeed, HIGH), digitalWrite(highSpeed, LOW); break;
          case 3: digitalWrite(lowSpeed, LOW), digitalWrite(medSpeed, LOW), digitalWrite(highSpeed, HIGH); break;
          default: digitalWrite(lowSpeed, HIGH); digitalWrite(medSpeed, LOW); digitalWrite(highSpeed, LOW);
            newModeCount = 0; break;
        }
        modeButtonCounter = newModeCount;
      }
    }

    delay(300);
  }
}
