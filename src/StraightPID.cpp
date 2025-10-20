//
// Created by joshg on 2025-10-19.
//
#ifndef STRAIGHT_PID_H
#define STRAIGHT_PID_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        /* Gains PID */
        float Kp;
        float Ki;
        float Kd;

        /* État interne PID */
        int32_t erreurPrec;
        float   sommeErreur;

        /* Constante roue: mm par pulse (roue 3" Robus) */
        float mm_par_pulse;
    } StraightPID;

    /* Initialisation (fait BoardInit + reset encodeurs) */
    void SPID_Init(StraightPID* spid);

    /* (Ré)initialise les encodeurs et l'état PID */
    void SPID_Reset(StraightPID* spid);

    /* Calcule la cible d’impulsions pour distance_m (mètres) */
    long SPID_CiblePulses(const StraightPID* spid, float distance_m);

    /* Applique la correction PID pour rouler droit à baseSpeed */
    void SPID_CorrigerVitesse(StraightPID* spid, float baseSpeed);

    /* Avance d’une distance en mètres à vitesse donnée, avec PID droit */
    void SPID_Avancer(StraightPID* spid, float distance_m, float vitesse);

    /* Même chose que avancer(), avec message série “Vers la quille !” */
    void SPID_VersQuille(StraightPID* spid, float distance_m, float vitesse);

#ifdef __cplusplus
}
#endif

#endif /* STRAIGHT_PID_H */
