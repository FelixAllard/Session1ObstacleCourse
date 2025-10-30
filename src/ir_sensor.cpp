#include "ir_sensor.h"
#include "LibRobus.h"
#include <math.h>

void IR_Init(void)
{

}

float IR_GetDistance(uint8_t id)
{
    uint16_t raw = ROBUS_ReadIR(id);


    float vout = (float)raw * 5.0f / 65535.0f;


    float distance = 27.86f * powf(vout, -1.15f);


    if (distance < 10.0f) distance = 10.0f;
    if (distance > 80.0f) distance = 80.0f;

    return distance;
}

bool IR_IsDetected(uint8_t id, float seuil_cm)
{
    float dist = IR_GetDistance(id);
    return (dist <= seuil_cm);
}
