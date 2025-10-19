//
// Created by joshg on 2025-10-19.
//

#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include <LibRobus.h>

class LineFollower {
public:
    LineFollower(uint8_t pinGauche, uint8_t pinMilieu, uint8_t pinDroite);

    void init();              // initialise les pins et les encodeurs
    void update();            // une itération de suivi de ligne
    void stop();              // arrête les moteurs
    void enable(bool state);  // active ou désactive le mode suiveur

private:
    uint8_t _pinGauche;
    uint8_t _pinMilieu;
    uint8_t _pinDroite;
    bool _enabled;

    // PID
    float _Kp;
    float _Ki;
    float _Kd;

    float _baseSpeed;
    float _maxSpeed;

    float _erreur;
    float _erreurPrec;
    float _sommeErreur;

    unsigned long _dernierContact;
    unsigned long _delaiPerte;
    bool _lignePerdue;

    void setVitesse(float gauche, float droite);
};

#endif
