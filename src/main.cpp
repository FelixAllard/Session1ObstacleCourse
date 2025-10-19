#include <LibRobus.h>
#include <LineFollow.h>

// --- Pin du suiveur de ligne ---
#define PIN_CAPTEUR_GAUCHE  22
#define PIN_CAPTEUR_MILIEU  24
#define PIN_CAPTEUR_DROITE  26

LineFollower suiveur(PIN_CAPTEUR_GAUCHE, PIN_CAPTEUR_MILIEU, PIN_CAPTEUR_DROITE);

// --- Permet d'activer ou désactiver si on veut suivre la ligne ---
int trackingMode = 0;   // 0 = OFF, 1 = ON

void setup() {
    BoardInit();
    suiveur.init();
    Serial.begin(9600);
    Serial.println("Robot prêt !");
}

void loop() {


    /*
    * Code pour activer ou désactiver le mode tracking selon si on fait une autre tache ou non
    trackingMode = 1;
    // --- Gestion du mode ---
    if (trackingMode == 1) {
        suiveur.enable(true);   // active le suiveur
        suiveur.update();       // fait une itération du suivi
    }
    else {
        suiveur.enable(false);  // stoppe les moteurs et le PID
    }
    */
}