//
// Created by xilef on 10/29/2025.
//

#include "LedControl.h"
#include <Arduino.h>
bool redLedOn = false;
bool greenLedOn = false;
bool blueLedOn = false;
bool yellowLedOn = false;

void SetupLEDS() {
    pinMode(redlight, OUTPUT);
    pinMode(yellowlight, OUTPUT);
    pinMode(greenlight, OUTPUT);
    pinMode(bluelight, OUTPUT);
}
//Turn LEDs on
void RedLEDOn() {
    digitalWrite(redlight,HIGH);
    redLedOn = true;
}
void YellowLEDOn() {
    digitalWrite(yellowlight,HIGH);
    yellowLedOn = true;
}
void GreenLEDOn() {
    digitalWrite(greenlight,HIGH);
    greenLedOn = true;
}
void BlueLEDOn() {
    digitalWrite(bluelight,HIGH);
    blueLedOn = true;
}

//Turn LEDs off
void RedLEDOff() {
    digitalWrite(redlight,LOW);
    redLedOn = false;
}
void YellowLEDOff() {
    digitalWrite(yellowlight,LOW);
    yellowLedOn = false;
}
void GreenLEDOff() {
    digitalWrite(greenlight,LOW);
    greenLedOn = false;
}
void BlueLEDOff() {
    digitalWrite(bluelight,LOW);
    blueLedOn = false;
}


//TRIGGERs
void RedLEDTrigger() {
    if (redLedOn)
        digitalWrite(redlight,LOW);
    else
        digitalWrite(redlight, HIGH);
    redLedOn = !redLedOn;

}
void YellowLEDTrigger() {
    if (yellowLedOn)
        digitalWrite(yellowlight,LOW);
    else
        digitalWrite(yellowlight, HIGH);
    yellowLedOn = !yellowLedOn;
}
void GreenLEDTrigger() {
    if (greenLedOn)
        digitalWrite(greenlight,LOW);
    else
        digitalWrite(greenlight, HIGH);
    greenLedOn = !greenLedOn;
}
void BlueLEDTrigger() {
    if (blueLedOn)
        digitalWrite(bluelight,LOW);
    else
        digitalWrite(bluelight, HIGH);
    blueLedOn = !blueLedOn;
}


//Random LEDs
void RandomLEDs() {
    long RanNum = random(0,10);

    if (RanNum == 0){ //Randomize LEDs on
        RedLEDTrigger();
    }

    if (RanNum == 1){
        YellowLEDTrigger();
    }

    if (RanNum == 2){
        GreenLEDTrigger();
    }

    if (RanNum == 3){
        BlueLEDTrigger();
    }

    if (RanNum == 4){
        RedLEDTrigger();
        YellowLEDTrigger();
    }

    if (RanNum == 5){
        RedLEDTrigger();
        GreenLEDTrigger();
    }

    if (RanNum == 6){
        RedLEDTrigger();
        BlueLEDTrigger();
    }

    if (RanNum == 7){
        YellowLEDTrigger();
        GreenLEDTrigger();
    }

    if (RanNum == 8){
        YellowLEDTrigger();
        BlueLEDTrigger();
    }

    if (RanNum == 9){
        GreenLEDTrigger();
        BlueLEDTrigger();
    }
}

void CloseAllLeds() {
    if (redLedOn)
        RedLEDOff();
    if (yellowLedOn)
        YellowLEDOff();
    if (greenLedOn)
        GreenLEDOff();
    if (blueLedOn)
        BlueLEDOff();
}