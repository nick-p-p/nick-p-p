#pragma once
#include <Arduino.h>
#include "LocalLanguage.h"
#include "LanguageDictionary.h"

// #define PHRASE_SAVED_SUCCESS    0
// #define PHRASE_BAD_ARGS         1
// #define PHRASE_TOPICS_REQUIRED  2
// #define PHRASE_SERVER_REQUIRED  3
// #define PHRASE_PHRASE_NOT_FOUND 4
// #define PHRASE_ARGS_MISSING     5
// #define PHRASE_WEATHER_STATION  6
// #define PHRASE_WEATHER_DISPLAY  7
// #define PHRASE_DOOR_CONTROLLER  8
// #define PHRASE_DOOR_WINDOW      9
// #define PHRASE_AIR_FRESHENER    10
// #define PHRASE_FROM_AFTER_TO    11
// #define PHRASE_HOTSPOT_TIMER    12
// #define PHRASE_CLOCK            13
// #define PHRASE_BATTERY_LOW      14
// #define PHRASE_OUTDOOR_SENSOR   15
// #define PHRASE_VOLTS            16
// #define PHRASE_BATTERY          17
// #define PHRASE_HVAC_SENSOR      18
// #define PHRASE_CALIBRATION_SENT 19
// #define PHRASE_INACTIVE         20
// #define PHRASE_MEDICATIONS      21
// #define PHRASE_LAST_TAKEN       22
// #define PHRASE_MED_BUTTONS      23
// #define PHRASE_NO_DATA          24
// #define PHRASE_ALERT_DISPLAY    25
// #define PHRASE_BLYNK_TOKEN      26
// #define PHRASE_PRESSURE         27
// #define PHRASE_INVALID_VALUE    28
// #define PHRASE_DELETED          29
// #define PHRASE_SPRAYED          30
// #define PHRASE_REBOOTING        31

// #define PHRASE_COUNT            32

#define INT_PHRASE_NO_DATA          -1
#define INT_PHRASE_PHRASE_NOT_FOUND -2
#define INT_PHRASE_ARGS_MISSING     -3
#define PHRASE_BAD_ARGS             -4
#define PHRASE_REBOOTING            -5
#define PHRASE_SAVED_SUCCESS        -6
#define PHRASE_NO_CHANGES           -7
#define INT_PHRASE_COUNT            7

class Language
{
public:
    Language();
    static void begin();//LanguageDictionary * langDictionary);
    static void setLanguage(localLang language);
    static localLang getLanguage();
    static const char* phrase(int phraseIdx);


private:
    static localLang currentLanguage;


    static LanguageDictionary langDictionary;
    static const String int_phrases[][INT_PHRASE_COUNT];
};

