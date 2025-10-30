#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>
#include <stdbool.h>


void IR_Init(void);

// Retourne la distance en centimètres (10–80 cm)
float IR_GetDistance(uint8_t id);

// Retourne vrai si un obstacle est détecté à moins d’un seuil donné
bool IR_IsDetected(uint8_t id, float seuil_cm);

#endif