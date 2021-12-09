#pragma once
#include <Arduino.h>
// #include "core/config/BoardsConfig.h"
#include <ILogger.h>

class TouchSensor
{

public:
    TouchSensor(int pin);
    void begin();
    bool touched();

private:
    long total = 30;
    int presses = 0;
    int pin;
    int threshold = 0;
    long lastMillisTouched = 0;
    bool touchedLastTime = false;

};