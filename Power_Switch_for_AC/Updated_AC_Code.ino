
/*  Duda.AC.v2.0
  This is a program to control a GE window air conditioning unit utlizing the built in pushbuttons 
  (connections scratched on the board to disconnect).  It will control power, compressor, and fan.
  Eventually it may integrate with thermostat.  For now it's ON or OFF.
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/

//  NOTE:  I need to add the music back to this code from the V1.2

// constants won't change. They're used here to set pin numbers:
const int powerButton = 1; 
const int modeButton = 3;  // can choose the fan speed
const int lowSpeed = 4;  //  Turns the fan on low
const int medSpeed = 5;  //  Turns the fan on medium
const int highSpeed = 6;  //  Turns the fan on high
const int speakerPin = 7;  //  The piezo buzzer is connected to this
const int compressorPin = 8;  //  This pin is connected to the relay the controls the compressor
const int thermostatPin = 9;  //To be used in the future when I read the thermostate via serial.

// variables will change:
int stateCompressorPin = LOW;     // the current state of LED
int lastButtonState;    // the previous state of button
int statePowerButton; // the current state of button
long time = 0;
long debounce = 200;
int modeButtonCounter = 0;   // counter for the number of button presses
int newModeCount;

void setup() {
  Serial.begin(9600);                // initialize serial
  pinMode(powerButton, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(compressorPin, OUTPUT);          // set arduino pin to output mode
  pinMode(lowSpeed, OUTPUT);
  pinMode(medSpeed, OUTPUT);
  pinMode(highSpeed, OUTPUT);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(thermostatPin, INPUT);
  statePowerButton = digitalRead(powerButton);
}

void loop() {
  lastButtonState = statePowerButton;      // save the last state
  statePowerButton = digitalRead(powerButton); // read new state

  if(lastButtonState == HIGH && statePowerButton == LOW) {
    Serial.println("The button is pressed");
    // toggle state of LED
    stateCompressorPin = !stateCompressorPin;
    digitalWrite(lowSpeed, HIGH);

    // control LED arccoding to the toggled state
    digitalWrite(compressorPin, stateCompressorPin);
    digitalWrite(lowSpeed, stateCompressorPin);
  }

if(digitalRead(modeButton)==LOW)  //  Need to add something here to make sure the AC unit is on.
{newModeCount=modeButtonCounter+1;
if(newModeCount!=modeButtonCounter)
{
Serial.println(newModeCount);
switch (newModeCount)
{ case 1: digitalWrite(lowSpeed,HIGH), digitalWrite(medSpeed,LOW), digitalWrite(highSpeed,LOW); break;
case 2: digitalWrite(lowSpeed,LOW), digitalWrite(medSpeed,HIGH), digitalWrite(highSpeed,LOW); break;
case 3: digitalWrite(lowSpeed,LOW), digitalWrite(medSpeed,LOW), digitalWrite(highSpeed,HIGH); break;
default: digitalWrite(lowSpeed,HIGH); digitalWrite(medSpeed,LOW); digitalWrite(highSpeed,LOW);
newModeCount=0; break; }
modeButtonCounter=newModeCount; }
}
delay(1000);
}
