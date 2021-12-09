#pragma once

#include <Arduino.h>
#include "FloatRange.h"
#include <ILogger.h>

class LightSensorBase
{
public:
    LightSensorBase(/* args */);

    /**
     * initialize the controller
     */
    void begin();

    /**
     * call frequently, reads the value and smoothes it
     */
    void loop();

    /**
     * returns the sensor value in range 0 to 255
     * based on the reading and upper & lower sensor values;
     */
    uint8_t scale255();

    /**
     * @brief set the analog reading upper value
     * @param value - the value of the sensor at which max brightness should be set
     */
    void setMin(int value);

    /**
     * @brief set the analog reading lower value
     * @param value - the value of the sensor at which min brightness should be set
     */
    void setMax(int value);

    /**
     * @brief set the analog reading upper value to the current sensor reading
     */
    void setMaxNow();

    /**
     * @brief set the analog reading upper value to the current sensor reading
     */
    void setMinNow();

protected:
    virtual float read() = 0;

    virtual FloatRange load() = 0;

    virtual void save(FloatRange) = 0;

    FloatRange range;


    Float smoothedReading = 0.0;

private:
};