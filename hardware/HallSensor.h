#pragma once
#include <Arduino.h>

class HallSensor
{

public:
    HallSensor(int upper, int lower);
    bool sensing();

private:
    int upper;
    int lower;    
};
