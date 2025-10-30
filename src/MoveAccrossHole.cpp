//
// Created by xilef on 10/29/2025.
//

#include "MoveAccrossHole.h"

#include <Arduino.h>

#include "turn.h"
#include "advance.h"

void MoveAccrossHole() {

    TurnDegrees(90);
    delay(200);
    moveDistanceBoth(35);
    delay(200);
    TurnDegrees(-93);
    delay(200);
    moveDistanceBoth(100);
}
