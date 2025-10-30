//
// Created by joshg on 2025-10-29.
//
#include "Whistle.h"
#include <stdio.h>
#include <Arduino.h>
#include <LibRobus.h>

int lastRead;
int result =0;
int count=0;
int count5Hz =0;
unsigned long lastUpdate = 0;
bool start =false;


void SetupWhistle() {
    pinMode(pinBruitAmbiant, INPUT) ;
    pinMode(pinIntensite_5HZ, INPUT) ;
}




bool SiffletStart() {
    unsigned long now0A0888 = millis();
    result = analogRead(pinBruitAmbiant) - analogRead(pinIntensite_5HZ);

    while (true)
    {

        if (result>60)
        {
            if (millis() - now0A0888 > 2000)
                return true;
        }else{
            return false;

        }
    }
}

void PrintToString() {
    Serial.println(analogRead(pinBruitAmbiant)) ;
    Serial.println(analogRead(pinIntensite_5HZ)) ;
    Serial.println(result = analogRead(pinBruitAmbiant) - analogRead(pinIntensite_5HZ)) ;

}

