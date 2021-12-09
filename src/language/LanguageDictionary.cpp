#include "LanguageDictionary.h"
#include <ArduinoJson.h>

LanguageDictionary::LanguageDictionary() {}

void LanguageDictionary::begin(){
    load();
}

char * LanguageDictionary::entry(int index, localLang language)
{
    if (index >= 0 && index < phrases.size())
    {
        return phrases.at(index)->texts[(int)language  ];
    }
    return notFound;
}

void LanguageDictionary::load()
{
    // Open file for reading
    File file = SPIFFS.open(fileName);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<4096> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        ILogger::log(l_error, LOG_TO_FILE, "Failed to read dictionary file error %d: %s", error.code(), error.c_str());

        return;
    }

    // extract the values
    JsonArray array = doc.as<JsonArray>();
    int phraseidx = 0;

    for (JsonVariant phr : array)
    {
        JsonArray texts = phr.as<JsonArray>();

        int idx = 0;

        Phrase*  phrase = new Phrase();;
        for (JsonVariant txt : texts)
        {
            String newText = txt.as<String>();
            if (newText == "" && idx > 0)
            {
                phrase->texts[idx] = phrase->texts[idx-1];
            }
            else
            {
                phrase->texts[idx] = new char[newText.length()+1];
                strncpy(phrase->texts[idx], newText.c_str(), newText.length() );
                phrase->texts[idx][newText.length()] = '\0';
            }
            idx++;
        }

        phrases.push_back(phrase);
        phraseidx++;
        
    }
    ILogger::log(l_info, LOG_TO_FILE, "Dictionary loaded %d entries", phraseidx);
    file.close();

    for( int i = 0;i < phrases.size(); i++)
    {
        Serial.print("EN_US: "); Serial.println(entry(i, lang_en_us));
        Serial.print("EN_UK: "); Serial.println(entry(i, lang_en_uk));
        Serial.print("EEP: "); Serial.println(entry(i, lang_es));
    }
}
