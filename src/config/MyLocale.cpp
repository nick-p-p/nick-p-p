#include "MyLocale.h"

localLang MyLocale::systemLanguage = lang_en_uk;

int MyLocale::systemLogLevel = (int)l_info;

bool MyLocale::systemUseCelsius = true;

MyLocale* MyLocale::instance;

MyLocale::MyLocale() : PageHandler(SYSTEM_LOCALE_UID)
{
}

void MyLocale::begin()
{
    if (!load())
    {
        save();
    }
    instance = new MyLocale();
    MyServer::subscribe(instance);
}

int MyLocale::getLogLevel()
{
	return systemLogLevel;
}

void MyLocale::setLogLevel(int debug)
{
	if (systemLogLevel != debug)
	{
		ILogger::setLevel((logLevel)debug);
		ILogger::log(l_debug, "message debug changed to: %d", debug);
		systemLogLevel = debug;
		
		save();
	}
}

bool MyLocale::useCelsius() { return systemUseCelsius; }

void MyLocale::setUseCelsius(bool useCelsius)
{
	if (systemUseCelsius != useCelsius)
	{
		ILogger::log(l_info, "use celsius changed to: %s", useCelsius ? "true" : "false");
		systemUseCelsius = useCelsius;
		save();
	}
}

void MyLocale::webPageData(AsyncWebServerRequest *request)
{
     PageProperties pageProperties(8000);

    if (!request->hasArg("div"))
    {
        pageProperties.sendMissingArgs(request, "div");
        return;
    }

    String uiSection = request->arg("div");

    ILogger::log(l_diagnostics, "In Locale::webPageData, div=%s", uiSection.c_str());

    if (uiSection == "buildinfo") // version and date on system menu
    {
        char buildTime[31];
        TZManager::format(buildTime, 30, BUILD_TIMESTAMP);

        pageProperties.add("sysver", FW_VERSION);
        pageProperties.add("compdate", buildTime);
        pageProperties.send(request);
    }
    else if (uiSection == "preferences")
    {
        pageProperties.add("tempunits", (int)systemUseCelsius);
        pageProperties.add("language", (int)Language::getLanguage());
        pageProperties.add("msgdebug", systemLogLevel);
        pageProperties.send(request);
    }
    // else if (uiSection == "webrootdownload") // files for file download page
    // {
    //     buildFileDownloads(request, "/web/", "webrootcontent", pageProperties);
    // }
    // else if (uiSection == "webendownload") // files for file download page
    // {
    //     buildFileDownloads(request, "/web/en/", "webencontent", pageProperties);
    // }
    // else if (uiSection == "webesdownload") // files for file download page
    // {
    //     buildFileDownloads(request, "/web/es/", "webescontent", pageProperties);
    // }
    // else if (uiSection == "datadownload") // files for file download page
    // {
    //     buildFileDownloads(request, "/", "datacontent", pageProperties);
    // }
    else
    {
        pageProperties.sendConfirmation(request, Language::phrase(PHRASE_BAD_ARGS));
    }
}


void MyLocale::savePageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(300);
    if (!request->hasArg("tempunits"))
    {
        pageProperties.sendMissingArgs(request, "tempunits");
        return;
    }

    if (!request->hasArg("msgdebug"))
    {
        pageProperties.sendMissingArgs(request, "msgdebug");
        return;
    }

    if (!request->hasArg("language"))
    {
        pageProperties.sendMissingArgs(request, "language");
        return;
    }

    setUseCelsius(request->arg("tempunits") != "0");
    setLogLevel((logLevel)request->arg("msgdebug").toInt());
    Language::setLanguage((localLang)request->arg("language").toInt());
    pageProperties.sendConfirmation(request, Language::phrase(PHRASE_SAVED_SUCCESS));
}

void MyLocale::save()
{

	ILogger::log(l_debug, "Saving locale settings");

	// Delete existing file, otherwise the configuration is appended to the file
	SPIFFS.remove(GLOBALS_JSON_FILE);

	// Open file for writing
	File globalsFile = SPIFFS.open(GLOBALS_JSON_FILE, FILE_WRITE);
	if (!globalsFile)
	{
		ILogger::log(l_error, "Failed to open locale config file for writing");
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
	doc["messageDebug"] = systemLogLevel;
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

bool MyLocale::load()
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
    systemLogLevel = 	doc["messageDebug"].as<int>();
	systemUseCelsius = doc["useCelsius"].as<bool>();
	systemLanguage = doc["language"].as<localLang>();

	globalsFile.close();


	ILogger::setLevel((logLevel)systemLogLevel);
    Language::setLanguage(systemLanguage);

    return true;
}
