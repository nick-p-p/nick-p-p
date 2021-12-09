#include "LightSensorBase.h"

LightSensorBase::LightSensorBase(){}

void LightSensorBase::begin()
{
    
    FloatRange loadedRange= load();
    floatRange.min = loadedRange.min;
    floatRange.max = loadedRange.max;

    ILogger::log(l_info, "light sensor min: %f, max: %f\r\n", floatRange.min, floatRange.max);
    loop();
}

void LightSensorBase::loop()
{
    smoothedReading = (smoothedReading * 15 + read()) / 16;
}

uint8_t LightSensorBase::scale255()
{
    float photoSensorValue = smoothedReading;

    if (photoSensorValue >= floatRange.max)
    {
        return 255;
    }

    if (photoSensorValue <=  floatRange.min)
    {
        return 0;
    }

    float range = floatRange.max - floatRange.min;
    float offset = photoSensorValue - floatRange.min;

    float scale1 = offset / range;
    uint8_t scale255 = (int)((scale * 255)+0.5);
    
    ILogger::log(l_debug, "photo sensor value: %f, scaled: %d\r\n", photoSensorValue, scale255);
    return  scale255;
}


void LightSensorBase::setMin(int value)
{
    floatRange.min = value;
    save(floatRange);
}

void LightSensorBase::setMax(int value)
{
    floatRange.max = value;
    save(floatRange);
}

void LightSensorBase::setMinNow()
{
    setMin(smoothedReading);
}

void LightSensorBase::setMaxNow()
{
    setMax(smoothedReading);
}
