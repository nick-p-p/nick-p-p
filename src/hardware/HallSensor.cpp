#include "HallSensor.h"

HallSensor::HallSensor(int upper, int lower)
{
    this->upper = upper;
    this->lower = lower;
}

bool HallSensor::sensing()
{
    int val = hallRead();
    bool sensed = val < lower || val > upper;
    ILogger::log(l_info, "Hall sensor red, value: %d => %ssensed", val, sensed ? "" : "not ");
    return sensed;
}