//
// Created by joshg on 2025-10-19.
//
#ifndef STRAIGHTPID_H
#define STRAIGHTPID_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        /* Gains PID */
        float Kp, Ki, Kd;

        /* État interne */
        int32_t erreurPrec;
        float   sommeErreur;

        /* Constante roue (mm par pulse) */
        float mm_par_pulse;
    } StraightPID;

    /* API C (équivalente aux méthodes C++) */
    void SPID_Init(StraightPID* spid);
    void SPID_InitCustom(StraightPID* spid, float kp, float ki, float kd, float mm_par_pulse);

    void SPID_InitEncoders(StraightPID* spid);  /* = init() */
    void SPID_Reset(StraightPID* spid);         /* = reset() */

    void SPID_Avancer(StraightPID* spid, float distance_m, float vitesse);
    void SPID_VersQuille(StraightPID* spid, float distance_m, float vitesse);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STRAIGHTPID_H */
