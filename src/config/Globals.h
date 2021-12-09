#pragma once
#include "SPIFFS.h"

#include "Arduino.h"
#include "ILogger.h"
// #include "Display.h"
// #include "Log.h"
// #include "stdlib.h"

#include <language.h>


/**
 * Version number of the software
 */ 
#ifndef FW_VERSION
#define FW_VERSION "?"
#endif 

#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__
#define COMPILE_TIME_EPOCH BUILD_TIMESTAMP

#define GLOBALS_JSON_FILE "/globals.json"


#ifdef BOARD_HAS_PSRAM
#include "core/PsramJson.h"
#else
#include "ArduinoJson.h"
#endif

class Globals 
{
public:
	Globals();

	/**
	 * Initialize and loads the global settings
	 * */
	static void begin();

	/**
	 * true if the detailed debugging of  messages is enabled
	 */
	static int messageDebug();
    
	/**
	 * Set whether detailed debugging of messages is enabled
	 */ 
	static void setMessageDebug(int debug);

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

private:
	static void save();
    static bool load();

	static int systemMessageDebugLevel;
	static bool systemUseCelsius;
    static localLang systemLanguage;

};
