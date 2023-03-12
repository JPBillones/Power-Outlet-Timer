#include <Wire.h>
#include <I2C_RTC.h>

static DS3231 RTC;

const int ledPin = 13;
const int resetButton = 4;
const int Relay1 = 11;
const int Relay2 = 12;
int Hour;
bool state, overrideMode;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 1;     // previous state of the button

void setup(){   
  RTC.begin();
  pinMode(resetButton, INPUT_PULLUP);
  relay(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Hour = RTC.getHours();
  if (Hour == 23 && overrideMode == 0){
    if(!state){
      state = 1;
      relay(state);
    }
  }
  manualButton();
  delay(50);
}

void relay(int relayState){
  if (relayState){ // Disconnect power
    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);
    digitalWrite(Relay1, LOW);
    digitalWrite(Relay2, LOW);
    digitalWrite(ledPin, HIGH);
  }else{// Reconnect power
    pinMode(Relay1, INPUT);
    pinMode(Relay2, INPUT);
    digitalWrite(ledPin, LOW);
  }
}

void manualButton(){
  buttonState = digitalRead(resetButton);
  if(buttonState != lastButtonState){
    buttonPushCounter++;
  }
  lastButtonState = buttonState;
  if (buttonPushCounter % 4 == 0) {
    relay(0);
    overrideMode = 0; // Reset override mode
  } else {
    relay(1);
    overrideMode = 1; // Override Timer and Turn On power
  }
}
