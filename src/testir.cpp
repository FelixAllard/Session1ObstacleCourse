//
// Created by joshg on 2025-10-19.
//

#include "ir_sensor.h"
#include "LibRobus.h"
#include <stdio.h>

int main(void)
{

    BoardInit();
    IR_Init();

    uint8_t id = 0;       // capteur IR branché sur IR0
    float distance;

    while (1)
    {
        distance = IR_GetDistance(id);

        if (IR_IsDetected(id, 20.0f))
        {
            printf("Obstacle détecté à %.1f cm\n", distance);
        }
        else
        {
            printf("Aucun obstacle (%.1f cm)\n", distance);
        }

        delay(200);
    }
}