//
// Created by xilef on 10/29/2025.
//

#ifndef SESSION1DANCINGROBOT_IRSENSOR_H
#define SESSION1DANCINGROBOT_IRSENSOR_H
#include <stdint.h>

float GetDistanceInfraRedSensor(uint8_t sensorID =0);
bool CheckIfObjectDetected(uint8_t sensorID =0);
void PrintIRDebug(uint8_t sensorID);
bool CheckIfObstacleInRange(uint8_t sensorID, float centimeters);
#endif //SESSION1DANCINGROBOT_IRSENSOR_H
