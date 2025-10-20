//
// Created by joshg on 2025-10-19.
//
#include "LineFollow.h"
#include <Arduino.h>
#include <LibRobus.h>

/* Tout le code ci-dessous a un linkage C et n'utilise QUE du C */
extern "C" {

/* Helpers C */
static inline float clampf(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static void set_vitesse(float gauche, float droite, float vmax) {
    gauche = clampf(gauche, -vmax, vmax);
    droite = clampf(droite, -vmax, vmax);
    MOTOR_SetSpeed(0, gauche);
    MOTOR_SetSpeed(1, droite);
}

/* Implémentation API */
void LF_Init(LineFollower* lf, uint8_t pinG, uint8_t pinM, uint8_t pinD)
{
    if (!lf) return;

    lf->pin_g = pinG;  lf->pin_m = pinM;  lf->pin_d = pinD;

    lf->enabled = false;
    lf->ligne_perdue = false;
    lf->dernier_contact_ms = 0;
    lf->delai_perte_ms = LF_LOST_DELAY_MS;

    lf->Kp = LF_KP_DEFAULT;  lf->Ki = LF_KI_DEFAULT;  lf->Kd = LF_KD_DEFAULT;
    lf->base_speed = LF_BASE_SPEED_DFLT;
    lf->max_speed  = LF_MAX_SPEED_DFLT;

    lf->err = 0.0f; lf->err_prec = 0.0f; lf->somme_err = 0.0f;

    pinMode(lf->pin_g, INPUT);
    pinMode(lf->pin_m, INPUT);
    pinMode(lf->pin_d, INPUT);

    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void LF_Enable(LineFollower* lf, bool state)
{
    if (!lf) return;
    lf->enabled = state;
    if (!state) LF_Stop(lf);
}

void LF_Stop(LineFollower* lf)
{
    (void)lf;
    MOTOR_SetSpeed(0, 0.0f);
    MOTOR_SetSpeed(1, 0.0f);
}

void LF_Update(LineFollower* lf)
{
    if (!lf || !lf->enabled) return;

    bool g = digitalRead(lf->pin_g);
    bool m = digitalRead(lf->pin_m);
    bool d = digitalRead(lf->pin_d);

    /* Calcul erreur */
    if (m)            lf->err = 0.0f;
    else if (g && !d) lf->err = -1.0f;
    else if (d && !g) lf->err =  1.0f;
    else if (g && d)  lf->err =  0.0f;
    else {
        if (!lf->ligne_perdue) {
            lf->dernier_contact_ms = millis();
            lf->ligne_perdue = true;
        }
        if ((millis() - lf->dernier_contact_ms) > lf->delai_perte_ms) {
            /* recherche : pivot anti-horaire doux */
            set_vitesse(-0.20f, 0.20f, lf->max_speed);
            return;
        } else {
            set_vitesse(0.0f, 0.0f, lf->max_speed);
            return;
        }
    }

    lf->ligne_perdue = false;

    /* PID discret */
    lf->somme_err += lf->err;
    float correction = (lf->Kp * lf->err)
                     + (lf->Ki * lf->somme_err)
                     + (lf->Kd * (lf->err - lf->err_prec));
    lf->err_prec = lf->err;

    float v_g = lf->base_speed - correction;
    float v_d = lf->base_speed + correction;

    set_vitesse(v_g, v_d, lf->max_speed);
}

void LF_SetGains(LineFollower* lf, float kp, float ki, float kd)
{
    if (!lf) return;
    lf->Kp = kp; lf->Ki = ki; lf->Kd = kd;
}

void LF_SetSpeeds(LineFollower* lf, float base_speed, float max_speed)
{
    if (!lf) return;
    lf->base_speed = base_speed;
    lf->max_speed  = (max_speed > 0.0f) ? max_speed : LF_MAX_SPEED_DFLT;
}

void LF_SetLostDelay(LineFollower* lf, unsigned long delay_ms)
{
    if (!lf) return;
    lf->delai_perte_ms = delay_ms;
}

} /* extern "C" */