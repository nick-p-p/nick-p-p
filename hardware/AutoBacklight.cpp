#include "AutoBacklight.h"

AutoBacklight::AutoBacklight(uint8_t brightnessSensorPin, uint8_t backlightLedPin, bool sensorInverted, bool backlightInverted, int EEPROMBaseAddress)
{
    config = EEPSettings(EEPROMBaseAddress);
    this->brightnessSensorPin = brightnessSensorPin;
    this->backlightLedPin = backlightLedPin;
    this->sensorInverted = sensorInverted;
    this->backlightInverted = backlightInverted;
    this->EEPROMBaseAddress = EEPROMBaseAddress;
}

void AutoBacklight::begin()
{
    config.load(true);
    pinMode(brightnessSensorPin, INPUT);
    pinMode(backlightLedPin, OUTPUT);
    analogWrite(backlightLedPin, backlightInverted ? 255 : 0);
}

void AutoBacklight::loop()
{
    int rawAnalog = sensorInverted
        ? 1023 - analogRead(brightnessResistorPin)
        : analogRead(brightnessResistorPin);

    smoothedReading = (smoothedReading * 15 + rawAnalog) / 16;

    //TODO write brightness
}

void AutoBacklight::setSensorLower(uint8_t value)
{
    config.brightnessSensorLowerValue = value;
    save();
    Serial.print("Light sesnsor min set to: ");
    Serial.println(value);
}

void AutoBacklight::setSensorUpper(uint8_t value)
{
    config.brightnessSensorUpperValue = value;
    svae();
    Serial.print("Light sensor max set to: ");
    Serial.println(value);
    brightnessResistorUpperValue = value;
    readEEPROM(EEPROMBase);
}

void AutoBacklight::setMaxLightNow()
{
    setSensorUpper(smoothedReading);
}

void AutoBacklight::setMinLightNow()
{
    setSensorLower(smoothedReading);
}