//
// Created by xilef on 10/27/2025.
//

#include "turn.h"

#include <Arduino.h>
#include <stddef.h>
#include <LibRobus.h>

#define ENCODER_PPR 3250.0f
#define WHEEL_DIAMETER 7.55f   // cm
#define WHEELBASE 18.0f       // cm
#define TURN_SPEED 0.20f      // constant turning speed


// Blocking turn: call once; function returns when the requested rotation is complete.
static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void TurnDegrees(float rotationDegrees) {
    if (rotationDegrees == 0.0f) return; // nothing to do

    // Determine direction
    bool turningRight = (rotationDegrees >= 0.0f);

    // Reset encoders
    ENCODER_Reset(0);
    ENCODER_Reset(1);

    delay(200);
    // Calculate pulses needed for the requested rotation
    const float wheelCirc = M_PI * WHEEL_DIAMETER;
    float turnDist = (M_PI * WHEELBASE) / (360.0f / fabs(rotationDegrees));
    float rotationsNeeded = turnDist / wheelCirc;
    float pulsesTarget = rotationsNeeded * ENCODER_PPR;

    // Configure motor speeds (clamped)
    float leftSpeed  = clampf(turningRight ?  TURN_SPEED : -TURN_SPEED, -1.0f, 1.0f);
    float rightSpeed = clampf(turningRight ? -TURN_SPEED :  TURN_SPEED, -1.0f, 1.0f);

    // Start turning
    MOTOR_SetSpeed(0, leftSpeed);
    MOTOR_SetSpeed(1, rightSpeed);

    // Poll encoders until target reached
    while (true) {
        long leftCount  = ENCODER_Read(0);
        long rightCount = ENCODER_Read(1);

        // Average distance in encoder pulses (absolute values)
        float avgDist = (fabs((float)leftCount) + fabs((float)rightCount)) / 2.0f;
        if (avgDist >= pulsesTarget) break;

        // Small delay to yield CPU and avoid loop jitter
        delay(1);
    }

    // Stop motors
    MOTOR_SetSpeed(0, 0.0f);
    MOTOR_SetSpeed(1, 0.0f);
}
