#include "Globals.h"

localLang Globals::systemLanguage = lang_en_uk;

int Globals::systemMessageDebugLevel = (int)l_info;

bool Globals::systemUseCelsius = true;

Globals::Globals()
{
}

void Globals::begin()
{
    if (!load())
    {
        save();
    }
}

int Globals::messageDebug()
{
	return systemMessageDebugLevel;
}

void Globals::setMessageDebug(int debug)
{
	if (systemMessageDebugLevel != debug)
	{
		ILogger::setLevel((logLevel)debug);
		ILogger::log(l_debug, "message debug changed to: %d", debug);
		systemMessageDebugLevel = debug;
		
		save();
	}
}

bool Globals::useCelsius() { return systemUseCelsius; }

void Globals::setUseCelsius(bool useCelsius)
{
	if (systemUseCelsius != useCelsius)
	{
		ILogger::log(l_info, "use celsius changed to: %s", useCelsius ? "true" : "false");
		systemUseCelsius = useCelsius;
		save();
	}
}

void Globals::save()
{

	ILogger::log(l_debug, "Saving global settings");

	// Delete existing file, otherwise the configuration is appended to the file
	SPIFFS.remove(GLOBALS_JSON_FILE);

	// Open file for writing
	File globalsFile = SPIFFS.open(GLOBALS_JSON_FILE, FILE_WRITE);
	if (!globalsFile)
	{
		ILogger::log(l_error, "Failed to open globals config file for writing");
		return;
	}

	// Allocate a temporary JsonDocument
	// Don't forget to change the capacity to match your requirements.
	// Use arduinojson.org/assistant to compute the capacity.
#ifdef BOARD_HAS_PSRAM    
	SpiRamJsonDocument doc(512);
#else
    StaticJsonDocument<512> doc;
#endif 

	// Set the values in the document
	doc["messageDebug"] = systemMessageDebugLevel;
	doc["useCelsius"] = systemUseCelsius;
	doc["language"] = systemLanguage;

	// Serialize JSON to file
	if (serializeJson(doc, globalsFile) == 0)
	{
		ILogger::log(l_error, "Failed to write to file for globals");
	}

	// Close the file
	globalsFile.close();

}

bool Globals::load()
{
	ILogger::log(l_info, "loading globals file: %s", GLOBALS_JSON_FILE);
    ILogger::log(l_info, "Free PSRAM: %d", ESP.getFreePsram());

    	// Open file for reading
	File globalsFile = SPIFFS.open(GLOBALS_JSON_FILE);
    if (!globalsFile)
    {
        ILogger::log(l_error, "Failed to read globals file %s, using default configuration", GLOBALS_JSON_FILE);
		return false;
    }

ILogger::log(l_info, "Free PSRAM: %d", ESP.getFreePsram());
	// Allocate a temporary JsonDocument
	// Don't forget to change the capacity to match your requirements.
	// Use arduinojson.org/v6/assistant to compute the capacity.
#ifdef BOARD_HAS_PSRAM    
	SpiRamJsonDocument doc(512);
#else
    StaticJsonDocument<512> doc;
#endif

	// Deserialize the JSON document
	DeserializationError error = deserializeJson(doc, globalsFile);
	if (error)
	{
		ILogger::log(l_error, "Failed to parse globals file %s, using default configuration", GLOBALS_JSON_FILE);
		return false;
	}

    ILogger::log(l_info, "Free PSRAM: %d", ESP.getFreePsram());

	// Copy values from the JsonDocument to the Config
	// Set the values in the document
    systemMessageDebugLevel = 	doc["messageDebug"].as<int>();
	systemUseCelsius = doc["useCelsius"].as<bool>();
	systemLanguage = doc["language"].as<localLang>();

	globalsFile.close();


	ILogger::setLevel((logLevel)systemMessageDebugLevel);
    return true;
}
