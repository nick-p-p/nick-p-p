#include "Language.h"

const String Language::int_phrases[2][INT_PHRASE_COUNT] =
    {
        {
            "No Data!",
            "Error: phrase not found in dictionary",
            "Argument missing: ",
            "Bad Args!",
            "System restarting",
            "Saved succesfully",
            "No changes"
        },
        {
            "No hay datos!",
            "Error: frase no encontrada en el diccionario",
            "falta el elemento: ",
            "Error con elementos",
            "Reinicio del sistema",
            "Exito!",
            "Nada cambie"
        }
    };
// const String Language::phrases[2][PHRASE_COUNT] =
//     {
//         {
//             "Saved succesfully",
//             "Bad Args!",
//             "If a server is specified, input and output topics cannot be blank",
//             "Server is required for active modes",
//             "Error: phrase not found in language module",
//             "Argument missing: ",
//             "Weather station",
//             "Weather display",
//             "Door and Window Controller",
//             "Doors and Windows",
//             "Air Freshener",
//             "From hour cannot be after to hour",
//             "Auto timer cannot be used in hotspot mode",
//             "Clock",
//             "Warning battery for %s at %d%%",
//             "Outside weather sensor",
//             "volts",
//             "battery",
//             "HVAC Pressure",
//             "Calibration sent!",
//             "Inactive",
//             "Medications",
//             "Last taken",
//             "Medication buttons",
//             "No data!",
//             "Alert Display",
//             "BLYNK token must be 32 characters or blank",
//             "Pressure",
//             "has an invalid value, please correct",
//             "deleted",
//             "Sprayed",
//             "System restarting"
//         },
//         {
//             "Exito!",
//             "Error",
//             "If a server is specified, input and output topics cannot be blank",
//             "Server is required for active modes",
//             "Error: phrase not found in language module",
//             "Argument missing: ",
//             "Weather station",
//             "Weather display",
//             "Door and Window Controller",
//             "Puertas y Ventanas",
//             "Air Freshener",
//             "From hour cannot be after to hour",
//             "Auto timer cannot be used in hotspot mode",
//             "Reloj",
//             "Advertencia: Batería para %s al %d%%",
//             "Temperatura afuera",
//             "voltas",
//             "Bater&iacute;a",
//             "Aire acondicionado",
//             "Calibration sent!",
//             "Inactive",
//             "Medicamentos",
//             "&uacute;ltima tomada",
//             "Botones de medicaci&oacute;n",
//             "No data!",
//             "Alert Display",
//             "BLYNK token must be 32 characters or blank",
//             "Pressure",
//             "has an invalid value, please correct",
//             "deleted",
//             "Sprayed",
//             "System restarting"
//         }
//      };

LanguageDictionary Language::langDictionary;

localLang Language::currentLanguage = lang_en_uk;

Language::Language() {}

void Language::begin() //LanguageDictionary * langDictionary)
{
    // Language::langDictionary = langDictionary;
    langDictionary.begin();
}

void Language::setLanguage(localLang language)
{
    currentLanguage = language;
}

localLang Language::getLanguage() { return currentLanguage; }

const char *Language::phrase(int phraseIdx)
{
    if (phraseIdx >= 0)
    {
        return langDictionary.entry(phraseIdx, currentLanguage);
    }
    else
    {
        int intPhraseIdx = -1 - phraseIdx;
        if (intPhraseIdx < INT_PHRASE_COUNT)
        {
            return (currentLanguage == lang_es) ? int_phrases[1][intPhraseIdx].c_str() : int_phrases[0][intPhraseIdx].c_str();
        }
        return phrase(INT_PHRASE_PHRASE_NOT_FOUND);
    }
}