//
// Created by xilef on 10/27/2025.
//
#include <Arduino.h>
#include <unity.h>

#include "advance.h"
#include "IRSensor.h"
#include "LedControl.h"
#include "librobus.h"
#include "ObstacleDestroyTarget.h"
#include "turn.h"
#include "Whistle.h"


void setUp(void) {
    // set stuff up here

}

void tearDown(void) {

}
void test_AllLEDs(void) {
    // Red LED
    RedLEDOn();
    delay(1000);           // see it turn on
    RedLEDOff();

    // Yellow LED
    YellowLEDOn();
    delay(1000);
    YellowLEDOff();

    // Blue LED
    BlueLEDOn();
    delay(1000);
    BlueLEDOff();

    // Green LED
    GreenLEDOn();
    delay(1000);
    GreenLEDOff();

    // Random LEDs
    for (int i = 0; i < 50; i++) {
        RandomLEDs();
        delay(50);
    }

    // Close all LEDs at the end
    CloseAllLeds();
    delay(200);

    // Assertion always passes (just a visual test)
    TEST_ASSERT_TRUE(true);
}
///Test whether we will hit the dang thing
void test_hitTheShit() {
    DestroyTarget();
}
void test_IRSensor() {
    while (true) {
        Serial.println(GetDistanceInfraRedSensor(2));
        delay(100);
    }

}
void test_whistle()
{
    while (true)
    {

        SiffletStart();

    }


}
void test_ValuesSensor() {

    while (true) {
        delay(100);
        Serial.println("__________________________________________________________");
        PrintToString();
    }
}

void test_YelloLine() {
    TurnDegrees(90);
    delay(100);
    moveDistanceBoth(60);
    delay(100);
    TurnDegrees(-90);
    delay(100);
    moveDistanceBoth(150);//62
    delay(100);
    TurnDegrees(-90);
    delay(100);
    moveDistanceBoth(50); //jusqua ce quon detecte la ligne
    delay(100);
    TurnDegrees(70);
}


int runUnityTests(void) {
    UNITY_BEGIN();

    //Test Leds
    //RUN_TEST(test_hitTheShit);
    //RUN_TEST(test_hitTheShit);
    //RUN_TEST(test_AllLEDs);
    //RUN_TEST(test_IRSensor);

    RUN_TEST(test_YelloLine);


    return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/*
/**
  * For native dev-platform or for some embedded frameworks
  #1#
int main(void) {
    return runUnityTests();
}
*/

/**
  * For Arduino framework
  */
void setup() {
    // Initialize board (motors, encoders, ArduinoX, Robus, etc.)
    BoardInit();
    delay(20); // allow hardware to settle
    Serial.begin(115200);
    SetupWhistle();
    SetupLEDS();
    Wire.begin();

    runUnityTests();
}
void loop() {}

/**
  * For ESP-IDF framework
  */
void app_main() {
    runUnityTests();
}