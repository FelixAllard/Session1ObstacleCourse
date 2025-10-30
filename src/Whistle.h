//
// Created by mathi on 2025-10-30.
//

#ifndef SESSION1OBSTACLECOURSE1_WHISTLE_H
#define SESSION1OBSTACLECOURSE1_WHISTLE_H

#include <pins_arduino.h>
const int pinIntensite_5HZ = A1 ;
const int pinBruitAmbiant = A0 ;
extern bool start;

void SetupWhistle() ;

bool SiffletStart() ;

void PrintToString();
#endif //SESSION1OBSTACLECOURSE_WHISTLE_H
