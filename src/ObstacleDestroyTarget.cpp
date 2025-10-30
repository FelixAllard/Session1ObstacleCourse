//
// Created by xilef on 10/29/2025.
//

#include "ObstacleDestroyTarget.h"
#include "advance.h"
#include "turn.h"

#include <Arduino.h>
#include <LibRobus.h>
#include "IRSensor.h"

#define ENCODER_PPR 3250.0f
#define WHEEL_DIAMETER 7.55f   // cm
#define WHEELBASE 18.0f       // cm
#define TURN_SPEED 0.15f      // constant turning speed

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void DestroyTarget() {
    moveDistanceBoth(10);
    float degreeTurned = TurnUntilAdvance();

    moveDistanceBoth(50);

    TurnDegrees(180);
    delay(200);
    moveDistanceBoth(50);
    delay(200);
    TurnDegrees(180-degreeTurned);




}

// Turn until CheckIfAdvance() returns true. Returns the angle actually turned (degrees)
float TurnUntilAdvance() {
    // Reset encoders
    ENCODER_Reset(0);
    ENCODER_Reset(1);
    delay(200);

    // Choose turning direction (example: always turn right; change if needed)
    bool turningRight = true;

    float leftSpeed  = clampf(turningRight ?  TURN_SPEED : -TURN_SPEED, -1.0f, 1.0f);
    float rightSpeed = clampf(turningRight ? -TURN_SPEED :  TURN_SPEED, -1.0f, 1.0f);

    MOTOR_SetSpeed(0, leftSpeed);
    MOTOR_SetSpeed(1, rightSpeed);

    while (!CheckIfObstacleInRange(0, 30)) {
        // wait for the condition
        delay(3);
    }

    // Stop motors
    MOTOR_SetSpeed(0, 0.0f);
    MOTOR_SetSpeed(1, 0.0f);

    // Calculate angle turned based on encoder counts
    long leftCount  = ENCODER_Read(0);
    long rightCount = ENCODER_Read(1);

    float wheelCirc = M_PI * WHEEL_DIAMETER;
    float leftDist  = ((float)leftCount / ENCODER_PPR) * wheelCirc;
    float rightDist = ((float)rightCount / ENCODER_PPR) * wheelCirc;
    float avgTurnDist = (fabs(leftDist) + fabs(rightDist)) / 2.0f;

    float angleTurned = (avgTurnDist / (M_PI * WHEELBASE)) * 360.0f; // degrees

    return turningRight ? angleTurned : -angleTurned;
}
