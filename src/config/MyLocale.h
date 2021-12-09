#pragma once
#include "SPIFFS.h"

#include "Arduino.h"
#include "ILogger.h"
#include "TZManager.h"

#include <language.h>
#include <PageHandler.h>

#define SYSTEM_LOCALE_UID 311

/**
 * Version number of the software
 */ 
#ifndef FW_VERSION
#define FW_VERSION "?"
#endif 

#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__
#define COMPILE_TIME_EPOCH BUILD_TIMESTAMP

#define GLOBALS_JSON_FILE "/locale.json"


#ifdef BOARD_HAS_PSRAM
#include "core/PsramJson.h"
#else
#include "ArduinoJson.h"
#endif

class MyLocale : public PageHandler
{
public:
	MyLocale();

	/**
	 * Initialize and loads the global settings
	 * */
	static void begin();

	/**
	 * true if the detailed debugging of  messages is enabled
	 */
	static int getLogLevel();
    
	/**
	 * Set whether detailed debugging of messages is enabled
	 */ 
	static void setLogLevel(int debug);

	/**
	 * true if displays and pages should use celsius
	 * internally all data is celsius
	 */ 
	static bool useCelsius();

	/**
	 * set whether displays and pages should use celsius
	 * internally all data is celsius
	 */ 
	static void setUseCelsius(bool useCelsius);

    /**
    *     SECTION FOR PAGEHANDLER OVERIDES
    */

    /**
     * @brief optional, return new layout to replace the file contents
     * called when action = "layout"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void webPageData(AsyncWebServerRequest *request);

    /**
     * @brief optional, save data sent from the page
     * called when action = "save"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void savePageData(AsyncWebServerRequest *request);



// -------------------------------------------------------------------------------

private:
	static void save();
    static bool load();

	static int systemLogLevel;
	static bool systemUseCelsius;
    static localLang systemLanguage;
	static MyLocale* instance;

};
