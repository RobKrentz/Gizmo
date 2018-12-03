//Based off Bare Conductive Light_Switch
//Use graphite as a capactive touch sensor to play musical notes

#include <MPR121.h> 
#include <Wire.h>

#include "pitches.h"

// notes to play, corresponding to the 12 sensors:
int notes[] = {
  NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2, NOTE_C2, NOTE_CS2,
};

//An octave Higher D4
int notes1[] = {
  NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4, NOTE_C4, NOTE_CS4,
};


#define numElectrodes 12

const int buttonPin = 11;
const int ledPin = 13; //change for feather

//Button to change octave
int buttonState = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT); //led to check button is working
  pinMode(buttonPin, INPUT); //button to change octave

  while (!Serial); // only needed if you want serial feedback with the
  // Arduino Leonardo or Bare Touch Board idk

  //if statement for serial print

  if (!MPR121.begin(0x5A)) {
    Serial.println("error setting up MPR121");
    switch (MPR121.getError()) {
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while (1);
  }

  // pin 6 to amp
  MPR121.setInterruptPin(6);

  // this is the touch threshold - setting it low makes it more like a proximity trigger
  // default value is 40 for touch
  MPR121.setTouchThreshold(40);

  // this is the release threshold - must ALWAYS be smaller than the touch threshold
  // default value is 20 for touch
  MPR121.setReleaseThreshold(20);

  // initial data update
  MPR121.updateTouchData();
}

//if statement for button on/off  
void loop()
{
  buttonState = digitalRead(buttonPin);

  // int sensorValue = analogRead(A0);
  // delay(1);

  if (buttonState == HIGH) {

    digitalWrite(ledPin, HIGH);                //turn led to check on
    // Serial.println("notes");

    if (MPR121.touchStatusChanged()) {         //if else statement for button switching between the two arrays to change 
      MPR121.updateTouchData();
      for (int i = 0; i < numElectrodes; i++) {
        if (MPR121.isNewTouch(i)) {
          Serial.print("electrode ");
          tone(6, notes[i], 20);
          Serial.print(i, DEC);
          Serial.println(" was just touched");
        } else if (MPR121.isNewRelease(i)) {
          Serial.print("electrode ");
          Serial.print(i, DEC);
          Serial.println(" was just released");
        }
      }
    }
  }
  else if (buttonState == LOW) {
    digitalWrite(ledPin, LOW); //turn led to check off
    // Serial.println("notes 1");

    if (MPR121.touchStatusChanged()) {
      MPR121.updateTouchData();
      for (int i = 0; i < numElectrodes; i++) {
        if (MPR121.isNewTouch(i)) {
          Serial.print("electrode ");
          tone(6, notes1[i], 20);
          Serial.print(i, DEC);
          Serial.println(" was just touched");
        } else if (MPR121.isNewRelease(i)) {
          Serial.print("electrode ");
          Serial.print(i, DEC);
          Serial.println(" was just released");
        }
      }
    }
  }
}
