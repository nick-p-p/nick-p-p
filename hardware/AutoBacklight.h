#pragma once
#include <Arduino.h>
#include <EEPSettings.h>

struct Backlightconfig
{
    int brightnessSensorUpperValue = 500;
    int brightnessSensorLowerValue = 10;

    int backLightMaxValue = 512;
    int backLightMinValue = 10;
;}

class AutoBacklight
{

public:
    /**
     * @brief ctor
     * 
     * @param brightnessSensorPin - aruidon pin number to which an LDR is connected, must be an analog input
     * @param backlightLedPin - pin for driving the backlight, must be PWM capable
     * @param sensorInverted - if true, then lower readings indicate more light
     * @param backlightInverted - if true then PWM is low for LED on
     * @param EEPROMBaseAddress - EEPROm address that this class can use
     */
    AutoBacklight(uint8_t brightnessSensorPin, uint8_t backlightLedPin, bool sensorInverted, bool backlightInverted, int EEPROMBaseAddress);

    void begin();

    void loop();

    /**
   * set the analog reading upper value
   * i.e. the value at whch max brightness should be set
   */
    void setSensorLower(uint8_t value);

    /** 
     * set the analog reading lower value
     * i.e. the value at which min brightness should be set
     */
    void setSensorUpper(uint8_t value);

    /**
   * set the analog reading upper value
   * i.e. the value at whch max brightness should be set
   * to the current light level
   */
    void setMaxLightNow();

    /**
     * set the analog reading lower value
     * i.e. the value at which min brightness should be set
     * to the current light level
     */
    void setMinLightNow();

private:
    /**
     * @brief calculates a new brightness level from the sensor taking into account all settings
     * value is 0 to 255
     */
    uint8_t calcScaledLevel();

    void save();

    bool load();

    uint8_t brightnessSensorPin;
    uint8_t backlightLedPin;
    bool sensorInverted;
    bool backlightInverted;
    int EEPROMBaseAddress;

    /**
     * reading smoothed over consecutive readings
     */
    int smoothedReading = 0;

    // /**
    //  * maximum reding for photocell i.e. the value at whch max brightness should be set
    //  */
    // int brightnessSensorUpperValue = 500;
    // /**
    //  * minimum reading for phototcell i.e. the value at which min brightness should be set
    //  */
    // int brightnessSensorLowerValue = 10;

    // int backLightMaxValue = 512;
    // int backLightMinValue = 10;

    EEPSettings<Backlightconfig> config;
};
