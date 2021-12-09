#include "HallSensor.h"

HallSensor::HallSensor(int upper, int lower)
{
    this->upper = upper;
    this->lower = lower;
}

bool HallSensor::sensing()
{
    int val = hallRead();
    return val < lower || val > upper;
}