//
// Created by joshg on 2025-10-19.
//

#include <LineFollow.h>

LineFollower::LineFollower(uint8_t pinGauche, uint8_t pinMilieu, uint8_t pinDroite)
  : _pinGauche(pinGauche), _pinMilieu(pinMilieu), _pinDroite(pinDroite)
{
    _Kp = 0.35;
    _Ki = 0.0;
    _Kd = 0.25;

    _baseSpeed = 0.4;
    _maxSpeed  = 0.8;

    _erreur = 0;
    _erreurPrec = 0;
    _sommeErreur = 0;

    _dernierContact = 0;
    _delaiPerte = 500;
    _lignePerdue = false;

    _enabled = false;
}

void LineFollower::init() {
    pinMode(_pinGauche, INPUT);
    pinMode(_pinMilieu, INPUT);
    pinMode(_pinDroite, INPUT);

    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void LineFollower::enable(bool state) {
    _enabled = state;
    if (!state) stop();
}

void LineFollower::setVitesse(float gauche, float droite) {
    if (gauche > _maxSpeed) gauche = _maxSpeed;
    if (droite > _maxSpeed) droite = _maxSpeed;
    if (gauche < -_maxSpeed) gauche = -_maxSpeed;
    if (droite < -_maxSpeed) droite = -_maxSpeed;

    MOTOR_SetSpeed(0, gauche);
    MOTOR_SetSpeed(1, droite);
}

void LineFollower::stop() {
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void LineFollower::update() {
    if (!_enabled) return;

    bool g = digitalRead(_pinGauche);
    bool m = digitalRead(_pinMilieu);
    bool d = digitalRead(_pinDroite);

    // Calcul erreur
    if (m) {
        _erreur = 0;
    } else if (g && !d) {
        _erreur = -1;
    } else if (d && !g) {
        _erreur = 1;
    } else if (g && d) {
        _erreur = 0;
    } else {
        if (!_lignePerdue) {
            _dernierContact = millis();
            _lignePerdue = true;
        }
        if (millis() - _dernierContact > _delaiPerte) {
            setVitesse(-0.2, 0.2); // tourne antihoraire
            return;
        }
    }

    _lignePerdue = false;

    // PID
    _sommeErreur += _erreur;
    float correction = (_Kp * _erreur) + (_Ki * _sommeErreur) + (_Kd * (_erreur - _erreurPrec));
    _erreurPrec = _erreur;

    float vitesseGauche = _baseSpeed - correction;
    float vitesseDroite = _baseSpeed + correction;

    setVitesse(vitesseGauche, vitesseDroite);
}
