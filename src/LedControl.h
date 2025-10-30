//
// Created by xilef on 10/29/2025.
//

#ifndef SESSION1OBSTACLECOURSE_LEDCONTROL_H
#define SESSION1OBSTACLECOURSE_LEDCONTROL_H
//LEDs (ajouter num pins)
const int redlight = 22 ;
const int yellowlight = 24;
const int greenlight = 26;
const int bluelight = 28;


extern bool redLedOn;
extern bool yellowLedOn;
extern bool greenLedOn;
extern bool blueLedOn;

// Setup
void SetupLEDS();

// LED Control - Turn ON
void RedLEDOn();
void YellowLEDOn();
void GreenLEDOn();
void BlueLEDOn();

// LED Control - Turn OFF
void RedLEDOff();
void YellowLEDOff();
void GreenLEDOff();
void BlueLEDOff();

//Randomize LEDs
void RandomLEDs();

void CloseAllLeds();
#endif //SESSION1OBSTACLECOURSE_LEDCONTROL_H