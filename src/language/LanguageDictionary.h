#pragma once
#include <Arduino.h>
#include "LocalLanguage.h"
#include <Phrase.h>
#include <vector>
#include <SPIFFS.h>
#include <ILogger.h>


class LanguageDictionary
{
public:
    LanguageDictionary(/* args */);
    void begin();
    // int maxEntry();
    char * entry(int index, localLang language);

private:
    std::vector<Phrase *> phrases;
    void load();

    const char fileName[17] = "/dictionary.json";

    char notFound[17] = "Phrase not found";
};
