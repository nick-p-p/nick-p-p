#pragma once
#include <Arduino.h>

#define MAGIC_BYTES 0x4C5567A0

template <typename T>
struct EEPData
{
public:
    unsigned long magicBytes = 1;
    T Settings;
};

class EEPSettings
{
    public:
    EEPSettings(uint16_t address );

    bool load(bool defaultIfInvalid = true);
    bool valid();
    void save();
    void reset();
    T& settings();

    private:
    uint16_t address = 0;
    EEPData<T> data;
}

template <typename T>
EEPSettings::EEPSettings(uint16_t address)
{
    this->address = address;
}

template <typename T>
bool EEPSettings::load(bool defaultIfInvalid)
{
    EEPROM.get(address, data);
    if (data.magicBytes != MAGIC_BYTES && defaultIfInvalid)
    {
        reset();
    }
    return data.magicBytes == MAGIC_BYTES;
}

template <typename T>
bool EEPSettings::valid()
{
    return data.magicBytes == MAGIC_BYTES;
}

template <typename T>
bool EEPSettings::save()
{
    data.magicBytes = MAGIC_BYTES;
    EEPROM.put(address, data);
    return data.magicBytes == MAGIC_BYTES;
}

template <typename T>
void EEPSettings<T>::reset()
{
    Serial.println(F("EEPROm RST"));
    T defaults;
    EEPROM.put(address + sizeof(unsigned long), defaults);
    EEPROM.get(address, data);
}

template <typename T>
T& EEPSettings<T>::settings()
{
    return data.Settings;
}