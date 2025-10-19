//
// Created by joshg on 2025-10-19.
//

#ifndef STRAIGHTPID_H
#define STRAIGHTPID_H

#include <LibRobus.h>

class StraightPID {
public:
    StraightPID();
    void init();
    void reset();
    void avancer(float distance_m, float vitesse);
    void versQuille(float distance_m, float vitesse);

private:
    float Kp, Ki, Kd;
    long ciblePulses(float distance_m);
    void corrigerVitesse(float baseSpeed);
};

#endif
