#include "TouchSensor.h"

TouchSensor::TouchSensor(int pin)
{
    this->pin = pin;
}

void TouchSensor::begin() {}

bool TouchSensor::touched()
{
    int cap = touchRead(pin);

    bool touched = cap < threshold;

    if (touched && !touchedLastTime)
    // check it says touched for 200ms (20 x 10ms)
    {
        int loops = 0;
        while (touched && (loops < 20))
        {
            loops++;
            touched = touchRead(pin) < threshold;
            delay(10);
        }
    }

    if (presses < 1000)
    {
        total += cap;
        presses++;
    }
    else
    {
        if (!touched)
        {
            total += cap;
            total = ((float)total) / 1.001;
        }
    }

    threshold = total / (presses * 2);

    touchedLastTime = touched;
    return touched;
}