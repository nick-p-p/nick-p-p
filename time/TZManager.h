#pragma once

#include <TimeZoneEntry.h>
#include <Arduino.h>
#include <sys/time.h>
#include "SPIFFS.h"
// #include "core/logging/DebugStream.h"
#include <PageProperties.h>
#include <StringBuilder.h>
#include <MyServer.h>
#include <ILogger.h>
#include <Language.h>

#define TZ_BAD_ARGS "Bad args!"
#define NTP_SERVER "pool.ntp.org"

//Jan 1 2016
#define MIN_VALID_EPOCH 1451649600

#define TIMEZONE_MANAGER_ID    300

/*
 Class for loading save and changing the timezone
*/
class TZManager : public PageHandler
{
public:
	TZManager();

	// initialize and load existing zone
	static TZManager * begin( bool connected);

	// wait for sntp to sync
	static void waitForTimeSet();

	// epoch time now
	static time_t now();

	// true if sntp is active and time is valid
	static bool timeSet();

	// return epoch offset to the local timezone
	static time_t localAsUTCEpoch();

	// minute in Local time zone
	static byte minuteLocal();

	// hour in Local time zone
	static byte hourLocal();

	// day of the month in Local time zine
	static byte dayOfMonth();

	// index of the time zone array that is currently active
	static int timeZoneIndex();

	// day of the year
	static int yday();

	/**
	 * write the local time to the given buffer in %c format
	 */
	static void format(char *buffer, int bufferLength, time_t t);

	/**
	 * write the local time to the given buffer in m/d H:M or D/m H:M depending on language
	 */
	static void formatMDHM(char *buffer, int bufferLength, time_t t);



private:
	// singleton
	static TZManager * instance;

	// the time zones available
	static TimeZoneEntry *timeZones[];

	// how many zones
	static int zoneCount;

	// calculate the local time offset from UTC
	static int calc_offset(time_t t);

	// when was the time (now) last split into components, used to avoid many calls to create timeinfo for the same second
	static time_t lastSplitTime;

	// utc time now
	static time_t t_now;

	// true if sntp is active and time is valid
	static bool _timeSet;

	// local time split out
	static struct tm timeinfoLocal;

	// function to split time into local fields
	static void splitTime();

	// active zone index
	static int zoneIndex;

	// set the timezone, save to SPIFFS and OS
	static void setTimeZone(int zoneIndex, bool doSave = true);
	/**
     * @brief optional, return new layout to replace the file contents
     * called when action = "layout"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */
	virtual void webPageContent(AsyncWebServerRequest *requestr);

	/**
    * @brief optional, return data to update the page
    * called when action = "data"
    * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */
	virtual void webPageData(AsyncWebServerRequest *request);


	// virtual int uniqueID();

	/**
     * @brief optional, save data sent from the page
     * called when action = "save"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */
	virtual void savePageData(AsyncWebServerRequest *request);

	// save the config
	static void save();

	//load the config
	static void load();


	static bool connected;
};