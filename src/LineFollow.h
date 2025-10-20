//
// Created by joshg on 2025-10-19.
//
#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

#include <stdint.h>
#include <stdbool.h>

/* Réglages par défaut */
#define LF_KP_DEFAULT        0.35f
#define LF_KI_DEFAULT        0.00f
#define LF_KD_DEFAULT        0.25f
#define LF_BASE_SPEED_DFLT   0.40f
#define LF_MAX_SPEED_DFLT    0.80f
#define LF_LOST_DELAY_MS     500UL

#ifdef __cplusplus
extern "C" {
#endif

    /* Handle C pur */
    typedef struct {
        /* IO */
        uint8_t pin_g;
        uint8_t pin_m;
        uint8_t pin_d;

        /* état */
        bool    enabled;
        bool    ligne_perdue;
        unsigned long dernier_contact_ms;
        unsigned long delai_perte_ms;

        /* PID */
        float   Kp, Ki, Kd;
        float   base_speed, max_speed;
        float   err, err_prec, somme_err;
    } LineFollower;

    /* API C */
    void LF_Init(LineFollower* lf, uint8_t pinG, uint8_t pinM, uint8_t pinD);
    void LF_Enable(LineFollower* lf, bool state);
    void LF_Update(LineFollower* lf);
    void LF_Stop(LineFollower* lf);

    void LF_SetGains(LineFollower* lf, float kp, float ki, float kd);
    void LF_SetSpeeds(LineFollower* lf, float base_speed, float max_speed);
    void LF_SetLostDelay(LineFollower* lf, unsigned long delay_ms);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LINE_FOLLOW_H */