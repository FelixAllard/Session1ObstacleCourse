//
// Created by joshg on 2025-10-19.
//

#include "StraightPID.h"

StraightPID::StraightPID() {
    Kp = 0.0015;    // Ajuste ces valeurs selon ton robot
    Ki = 0.0;
    Kd = 0.001;

    BoardInit();
}

void StraightPID::init() {
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void StraightPID::reset() {
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

long StraightPID::ciblePulses(float distance_m) {
    const float mm_par_pulse = 76.2 / 3200.0; // roue 3" Robus
    return (distance_m * 1000.0) / mm_par_pulse;
}

void StraightPID::corrigerVitesse(float baseSpeed) {
    int32_t g = ENCODER_Read(0);
    int32_t d = ENCODER_Read(1);

    static int32_t erreurPrec = 0;
    static float sommeErreur = 0;

    int32_t erreur = g - d;
    sommeErreur += erreur;
    float deriv = erreur - erreurPrec;

    float correction = Kp * erreur + Ki * sommeErreur + Kd * deriv;

    float vitesseG = baseSpeed - correction;
    float vitesseD = baseSpeed + correction;

    MOTOR_SetSpeed(0, vitesseG);
    MOTOR_SetSpeed(1, vitesseD);

    erreurPrec = erreur;
}

void StraightPID::avancer(float distance_m, float vitesse) {
    reset();
    long cible = ciblePulses(distance_m);

    while (((ENCODER_Read(0) + ENCODER_Read(1)) / 2) < cible) {
        corrigerVitesse(vitesse);
        delay(10);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void StraightPID::versQuille(float distance_m, float vitesse) {
    Serial.println("Vers la quille !");
    avancer(distance_m, vitesse);
}
