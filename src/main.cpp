#include <Arduino.h>
#include <Wire.h>
#include <turn.h>
#include <advance.h>
#include <ir_sensor.h>
#include <LibRobus.h>

#include "LineFollow.h"
#include "LedControl.h"
#include "MoveAccrossHole.h"
#include "ObstacleDestroyTarget.h"
#include "Whistle.h"
// TCS34725 Color Sensor - Registres et constantes
#define TCS34725_ADDR          0x29
#define TCS34725_COMMAND_BIT   0x80
#define TCS34725_ENABLE        0x00
#define TCS34725_ENABLE_AIEN   0x10    // RGBC Interrupt Enable
#define TCS34725_ENABLE_WEN    0x08     // Wait enable - Writing 1 activates the wait timer
#define TCS34725_ENABLE_AEN    0x02     // RGBC Enable - Writing 1 actives the ADC, 0 disables it
#define TCS34725_ENABLE_PON    0x01    // Power on - Writing 1 activates the internal oscillator, 0 disables it

#define TCS34725_ATIME         0x01    // Integration time
#define TCS34725_WTIME         0x03    // Wait time
#define TCS34725_CONTROL       0x0F    // Set the gain level for the sensor
#define TCS34725_ID            0x12    // 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727
#define TCS34725_STATUS        0x13
#define TCS34725_CDATAL        0x14    // Clear channel data
#define TCS34725_CDATAH        0x15
#define TCS34725_RDATAL        0x16    // Red channel data
#define TCS34725_RDATAH        0x17
#define TCS34725_GDATAL        0x18    // Green channel data
#define TCS34725_GDATAH        0x19
#define TCS34725_BDATAL        0x1A    // Blue channel data
#define TCS34725_BDATAH        0x1B

bool didRed = false;
bool didBlue = false;
bool didGreen = false;
bool didYellow = false;


// Fonction utilitaire pour écrire un registre
void writeReg(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(TCS34725_ADDR);
    Wire.write(TCS34725_COMMAND_BIT | reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Fonction utilitaire pour lire un registre
uint8_t readReg(uint8_t reg) {
    Wire.beginTransmission(TCS34725_ADDR);
    Wire.write(TCS34725_COMMAND_BIT | reg);
    Wire.endTransmission();
    
    Wire.requestFrom(TCS34725_ADDR, 1);
    return Wire.read();
}

// Fonction utilitaire pour lire 16 bits
uint16_t read16(uint8_t reg) {
    uint16_t x;
    uint16_t t;

    Wire.beginTransmission(TCS34725_ADDR);
    Wire.write(TCS34725_COMMAND_BIT | reg);
    Wire.endTransmission();

    Wire.requestFrom(TCS34725_ADDR, 2);
    t = Wire.read();
    x = Wire.read();
    x <<= 8;
    x |= t;
    return x;
}

void setup() {

    // Initialize board (motors, encoders, ArduinoX, Robus, etc.)
    BoardInit();
    delay(20); // allow hardware to settle
    Serial.begin(115200);
    Wire.begin();
    LineFollowSetup();
    SetupLEDS();
    SetupWhistle();

    // Scanner I2C pour vérifier
    Serial.println("\nScanning I2C bus...");
    byte error, address;
    int nDevices = 0;
    
    for(address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            nDevices++;
        }
    }
    
    if (nDevices == 0) {
        Serial.println("No I2C devices found");
    } else {
        Serial.println("Scan complete\n");
    }

    // Vérifier l'ID du capteur
    uint8_t id = readReg(TCS34725_ID);
    Serial.print("Device ID: 0x");
    Serial.println(id, HEX);
    
    if (id != 0x44 && id != 0x4D) {
        Serial.println("Error: Sensor ID incorrect!");
        while(1);
    }

    // Power ON
    writeReg(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    delay(3);
    writeReg(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);

    // Configuration temps d'intégration (154ms)
    writeReg(TCS34725_ATIME, 0xEB);

    // Configuration gain (16x)
    writeReg(TCS34725_CONTROL, 0x02);

    Serial.println("Color sensor initialized!");
}

void getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    *c = read16(TCS34725_CDATAL);
    *r = read16(TCS34725_RDATAL);
    *g = read16(TCS34725_GDATAL);
    *b = read16(TCS34725_BDATAL);
}

// Structure pour stocker une couleur RGB
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

int couleur;
void getColor() {
    
    uint16_t r, g, b, c;
    getRawData(&r, &g, &b, &c);

    // Éviter la division par zéro et normaliser
    if(c == 0) c = 1;
    float rf = min(255.0f, (r / (float)c) * 255.0f);
    float gf = min(255.0f, (g / (float)c) * 255.0f);
    float bf = min(255.0f, (b / (float)c) * 255.0f);

    // Convertir en couleur RGB 8-bit
    Color color = {
        (uint8_t)rf,
        (uint8_t)gf,
        (uint8_t)bf
    };
    Serial.println("-----------------------------------");
    Serial.print("rf : ");
    Serial.println(color.r);
    Serial.print("gf : ");
    Serial.println(color.g);
    Serial.print("bf : ");
    Serial.println(color.b);

    if (abs(color.r-70) <= 1.0 && abs(color.g-88) <= 1.0 && abs(color.b-74) <= 1.0 && didBlue == false) {
        couleur = 1;
        BlueLEDOn();
        didBlue = true;
    } //bleu
    else if (abs(color.r-71) <= 1.0 && abs(color.g-90) <= 1.0 && abs(color.b-69) <= 1.0 && didGreen == false) {
        couleur = 2;
        GreenLEDOn();
        didGreen = true;
    } //vert
    else if (abs(color.r-85) <= 2 && abs(color.g-87) <= 2 && abs(color.b-60) <= 2 && didYellow == false) {
        couleur = 3;
        YellowLEDOn();
        didYellow = true;
    } //jaune
    else if (abs(color.r-85) <= 1.0 && abs(color.g-78) <= 1.0 && abs(color.b-70) <= 1.0 && didRed == false) {
        couleur = 4;
        RedLEDOn();
        didRed = true;
    } //rose
    else {
        couleur = 0;
        CloseAllLeds();
    } //aucune
    
}

void loop() {
    getColor();
    Serial.println(couleur);


    if(couleur == 1) { //bleu
        moveDistanceBoth(43);
        delay(150);
        TurnDegrees(-135);
        delay(150);
        moveDistanceBoth(50);
        for(int i=0; i<2; i++) {
            delay(150);
            TurnDegrees(-90);
            delay(150);
            moveDistanceBoth(50);
        }
        delay(150);
        TurnDegrees(-90);
        delay(150);
        moveDistanceBoth(20);

        while (!isLineDetected()) {
            updateFollower();  // continue à corriger légèrement la direction
            moveDistanceBoth(2);
            delay(10);
        }
        TurnDegrees(30);


    }

    if(couleur == 2) { //vert
        MoveAccrossHole();
    }

    if(couleur == 3) { //jaune
        TurnDegrees(90);
        delay(100);
        moveDistanceBoth(60);
        delay(100);
        TurnDegrees(-90);
        delay(100);
        moveDistanceBoth(62);
        delay(100);
        TurnDegrees(-90);
        delay(100);
        moveDistanceBoth(50); //jusqua ce quon detecte la ligne
        delay(100);
        TurnDegrees(70);
    }

    if(couleur == 4) { //rose
        DestroyTarget();
    }
    updateFollower();

}