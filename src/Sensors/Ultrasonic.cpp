#include "CnPD.h"

void setupUltrasonicSensors() {
    pinMode(US_FRONT[0], OUTPUT);
    pinMode(US_FRONT[1], INPUT);

    pinMode(US_LEFT[0], OUTPUT);
    pinMode(US_LEFT[1], INPUT);

    pinMode(US_RIGHT[0], OUTPUT);
    pinMode(US_RIGHT[1], INPUT);
}

bool isObstacleFront(int thresholdDistanceCm) {
    long duration;
    int distanceCm;

    digitalWrite(US_FRONT[0], LOW);
    delayMicroseconds(2);
    digitalWrite(US_FRONT[0], HIGH);
    delayMicroseconds(10);
    digitalWrite(US_FRONT[0], LOW);

    duration = pulseIn(US_FRONT[1], HIGH);

    distanceCm = duration * 0.034 / 2;

    return distanceCm < thresholdDistanceCm;
}