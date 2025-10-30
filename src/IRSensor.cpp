//
// Created by xilef on 10/29/2025.
//

#include "IRSensor.h"
#include <LibRobus.h>

struct IRCal {
    float voltage;   // measured sensor voltage
    float distance;  // corresponding distance in cm
};

// Example calibration table (adjust for your specific sensor)
IRCal irTable[] = {
    {2.8, 10},
    {2.0, 15},
    {1.6, 20},
    {1.2, 30},
    {1.0, 40},
    {0.8, 50},
    {0.6, 70},
    {0.5, 80}
};
const int tableSize = sizeof(irTable)/sizeof(IRCal);

float voltageToDistance(float V) {
    // Out of range
    if(V >= irTable[0].voltage) return irTable[0].distance;
    if(V <= irTable[tableSize-1].voltage) return irTable[tableSize-1].distance;

    // Linear interpolation
    for(int i = 0; i < tableSize - 1; i++) {
        if(V <= irTable[i].voltage && V >= irTable[i+1].voltage) {
            float t = (V - irTable[i+1].voltage) / (irTable[i].voltage - irTable[i+1].voltage);
            return irTable[i+1].distance + t * (irTable[i].distance - irTable[i+1].distance);
        }
    }
    return 0; // safety fallback
}

float GetDistanceInfraRedSensor(uint8_t sensorID) {
    uint16_t raw = ROBUS_ReadIR(sensorID);
    // Convert to voltage
    float V = raw * (5.0 / 65535.0);

    // Convert to distance using GP2D12 formula
    float distance = 27.86 / pow(V, 1.15);


    distance /= 120;
    if (distance>70)
        return 70;
    return distance;
}

bool CheckIfObstacleInRange(uint8_t sensorID, float centimeters) {

    uint16_t raw = ROBUS_ReadIR(sensorID);
    // Convert to voltage
    float V = raw * (5.0 / 65535.0);

    // Convert to distance using GP2D12 formula
    float distance = 27.86 / pow(V, 1.15);


    distance /= 120;
    if (distance>70)
        distance = 70;
    if (distance < centimeters)
        return true;
    return false;
}
bool CheckIfObjectDetected(uint8_t sensorID) {
    uint16_t raw = ROBUS_ReadIR(sensorID);
    if(raw < 100 || raw > 65000) {
        return false;
    }

    // Convert raw value to voltage
    float V = raw * (5.0 / 65535.0);
    if (V < 0.4) V = 0.4;

    // Convert voltage to distance
    float distance = 27.86 / pow(V, 1.15);

    // Clamp distance minimally
    if (distance < 10) distance = 10;

    // --- Smoothing ---
    static bool firstCall = true;
    static float lastDistance;
    if(firstCall) {
        lastDistance = distance;  // initialize on first reading
        firstCall = false;
    }
    float alpha = 0.3;
    lastDistance = alpha * distance + (1 - alpha) * lastDistance;
    distance = lastDistance;

    Serial.println(distance);

    // --- Object detection threshold ---
    const float DETECTION_THRESHOLD = 50.0; // cm
    return distance < DETECTION_THRESHOLD;
}

void PrintIRDebug(uint8_t sensorID) {
    uint16_t raw = ROBUS_ReadIR(sensorID);  // raw sensor value 0-65535

    // Convert to voltage
    float V = raw * (5.0 / 65535.0);

    // Convert to distance using GP2D12 formula
    float distance = 27.86 / pow(V, 1.15);

    // Print everything
    Serial.print("Sensor "); Serial.print(sensorID);
    Serial.print(" | Raw: "); Serial.print(raw);
    Serial.print(" | V: "); Serial.print(V, 3);
    Serial.print(" V | Distance: "); Serial.print(distance, 2);
    Serial.println(" cm");
}
