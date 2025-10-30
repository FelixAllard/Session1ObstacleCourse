#include "LineFollow.h"
#include <Arduino.h>
#include <LibRobus.h>

// === CAPTEURS ===
#define LEFT_SENSOR    A3
#define CENTER_SENSOR  A4
#define RIGHT_SENSOR   A5

// === MOTEURS ===
#define MOTOR_LEFT   0
#define MOTOR_RIGHT  1

// === CONSTANTES ===
#define BASE_SPEED   0.15
#define TURN_ADJUST  0.27
#define THRESHOLD  960


// === FONCTIONS ===

void LineFollowSetup() {
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(CENTER_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  Serial.println("=== INITIALISATION CAPTEURS ===");
}

void updateFollower() {

  int leftVal = analogRead(LEFT_SENSOR);
  int centerVal = analogRead(CENTER_SENSOR);
  int rightVal = analogRead(RIGHT_SENSOR);


  bool left = (leftVal > THRESHOLD);
  bool center = (centerVal > THRESHOLD);
  bool right = (rightVal > THRESHOLD);


  if (center && !left && !right) {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED);
  }
  else if (left && !center) {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED - TURN_ADJUST);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED + TURN_ADJUST);
  }
  else if (right && !center) {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED + TURN_ADJUST);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED - TURN_ADJUST);
  }
  else if (left && center && !right) {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED - TURN_ADJUST / 2);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED + TURN_ADJUST / 2);
  }
  else if (right && center && !left) {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED + TURN_ADJUST / 2);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED - TURN_ADJUST / 2);
  }
  else {

    MOTOR_SetSpeed(MOTOR_LEFT, BASE_SPEED);
    MOTOR_SetSpeed(MOTOR_RIGHT, BASE_SPEED);
  }


  Serial.print("L:"); Serial.print(leftVal);
  Serial.print(" C:"); Serial.print(centerVal);
  Serial.print(" R:"); Serial.print(rightVal);
  Serial.println();



}

void testCapteursDetail() {
  Serial.println("=== TEST CAPTEURS ANALOGIQUES ===");
  Serial.print("Gauche (A3): "); Serial.println(analogRead(LEFT_SENSOR));
  Serial.print("Centre (A4): "); Serial.println(analogRead(CENTER_SENSOR));
  Serial.print("Droite (A5): "); Serial.println(analogRead(RIGHT_SENSOR));
  Serial.println("Placez les capteurs sur noir et blanc pour ajuster le seuil.");
  delay(2000);

}
