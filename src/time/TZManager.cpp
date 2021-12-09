#include "TZManager.h"

//0 - United Kingdom (London, Belfast)
TimeZoneEntry UKtz("(GMT): United Kingdom", "GMT0BST,M3.5.0/1:00:00,M10.5.0/2:00:00");

//6 - Central European Time (Frankfurt, Paris)
TimeZoneEntry CEtz("(GMT +1): Central Europe", "CET-1CEST,M3.5.0/2,M10.5.0/3");

//1 - US Eastern Time Zone (New York, Detroit)
TimeZoneEntry usETtz("(GMT -5): Eastern US", "EST5EDT,M3.2.0/2,M11.1.0");

//2 - US Central Time Zone (Chicago, Houston)
TimeZoneEntry usCTtz("(GMT -6): Central US", "CST6CDT,M3.2.0/2,M11.1.0/2");

//3 - US Mountain Time Zone (Denver, Salt Lake City)
TimeZoneEntry usMTtz("(GMT -7): Mountain US", "MST7MDT,M3.2.0/2,M11.1.0/2");

TimeZoneEntry usPTtz("(GMT -8): Pacific US", "PST8PDT,M3.2.0/2,M11.1.0/2");

//5 - Arizona is US Mountain Time Zone but does not use DST
TimeZoneEntry usAZtz("(GMT -7): Arizona", "AZT7");

//7 - Colombia
TimeZoneEntry COLtz("(GMT -5): Colombia", "COL5");

//statics
int TZManager::zoneCount = 8;
int TZManager::zoneIndex = 0;

bool TZManager::_timeSet = false;
time_t TZManager::t_now = 0;
time_t TZManager::lastSplitTime = 0;
struct tm TZManager::timeinfoLocal;
TZManager *TZManager::instance = nullptr;
bool TZManager::connected = false;

TimeZoneEntry *TZManager::timeZones[] = {&UKtz, &CEtz, &usETtz, &usCTtz, &usMTtz, &usPTtz, &usAZtz, &COLtz};

TZManager::TZManager() : PageHandler(TIMEZONE_MANAGER_ID)
{
	_timeSet = false;
}

TZManager *TZManager::begin( bool connected)
{
	instance = new TZManager();
	TZManager::connected = connected;

	if (connected)
	{
		ILogger::log(l_info, "setting time to GMT and NTP to pool.ntp.org");
		configTime(0, 0, NTP_SERVER);
	}

	ILogger::log(l_info, "loading settings");
	load();

	waitForTimeSet();

	setTimeZone(zoneIndex, false);

	MyServer::subscribe(instance);

	return instance;
}

void TZManager::waitForTimeSet()
{
	if (!connected)
	{
		return;
	}

	// struct tm timeinfo;
	time(&t_now);
	// localtime_r(&t_now, &timeinfo);
	// Is time set? check > Jan 2016

	while (t_now < MIN_VALID_EPOCH)
	{
		delay(500);
		time(&t_now);
	}

	// while (timeinfo.tm_year < (2016 - 1900))
	// {
	// 	debugStream->printf("Time is not set yet. year = %d\r\n", timeinfo.tm_year);
	// 	_timeSet = false;
	// 	delay(500);
	// 	// update 'now' variable with current time
	// 	time(&t_now);
	// 	localtime_r(&t_now, &timeinfo);
	// }

	_timeSet = true;
}

byte TZManager::minuteLocal()
{
	splitTime();
	return timeinfoLocal.tm_min;
}

byte TZManager::hourLocal()
{
	splitTime();
	return timeinfoLocal.tm_hour;
}

byte TZManager::dayOfMonth()
{
	splitTime();
	return timeinfoLocal.tm_mday;
}

time_t TZManager::now()
{
	time(&t_now);
	_timeSet = (t_now > 1546300800);

	return _timeSet ? t_now : 0;
}

bool TZManager::timeSet()
{
	return _timeSet;
}

void TZManager::webPageContent(AsyncWebServerRequest *request)
{

	int zonesToInclude = zoneCount;
#ifdef ARDUINO_ESP32_POE_ISO
	zonesToInclude--;
#endif

	StringBuilder stringBuilder(512);
	stringBuilder.append("[ {\"zoneselection\" : \"")
		.append("<select name='tzone' id='tzone'>");

	for (int i = 0; i < zonesToInclude; i++)
	{
		stringBuilder.append("<option value='%d'>%s</option>",
							 i,
							 timeZones[i]->Description());
	}

	stringBuilder.append("</select>")
		.append("\"}]");

	request->send(200, "text/json", stringBuilder.data());
}

void TZManager::webPageData(AsyncWebServerRequest *request)
{
	PageProperties pageProperties(128);
	pageProperties.add("tzone", zoneIndex);
	pageProperties.send(request);
}

// int TZManager::uniqueID()
// {
// 	return TIMEZONE_MANAGER_ID;
// }

void TZManager::setTimeZone(int newZoneIndex, bool doSave)
{
	zoneIndex = newZoneIndex;
	ILogger::log(l_diagnostics, timeZones[zoneIndex]->Description());
	ILogger::log(l_diagnostics, timeZones[zoneIndex]->Timezone());
	if (doSave)
	{
		save();
	}
	setenv("TZ", timeZones[zoneIndex]->Timezone(), 1);

	tzset();

	char strftime_buf[64];
	time_t t_now;
	struct tm timeinfo;
	time(&t_now);
	localtime_r(&t_now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
#ifdef Spanish
	ILogger::log(l_info, "La fecha/hora local actual es: %s", strftime_buf);
#else
	ILogger::log(l_info, "The current date/time local is: %s\r\n", strftime_buf);
	ILogger::log(l_info, "Calculating offset from UTC...");
	calc_offset(t_now);
#endif
}

void TZManager::savePageData(AsyncWebServerRequest *request)
{
	String result = "Error";
	PageProperties pageProperties(128);

	if (!request->hasArg("tzone"))
	{
		pageProperties.sendMissingArgs(request, "tzone");
		return;
	}

	String zone = request->arg("tzone");
	ILogger::log(l_info, "new time zone %s\r\n", zone.c_str());

	int newZoneIndex = zone.toInt();

	// CHECK IF SAME ZONE RETURNED
	if (newZoneIndex == zoneIndex)
	{
		result = (Language::getLanguage() == lang_es)
					 ? "Nada cambie"
					 : "No changes";
	}
	else
	{
		// CHECK IF INVALID	 ZONE RETURNED
		if ((newZoneIndex < 0) || (newZoneIndex >= zoneCount))
		{
			result = (Language::getLanguage() == lang_es)
						 ? "No se puede establecer la zona horaria inv&Aacute;lida, falla..."
						 : "Unable to set timezone to invalid zone, defaulting...";
		}
		else
		{
			setTimeZone(newZoneIndex);
			ILogger::log(l_info, "saved zone: %d\r\n", newZoneIndex);
			result = (Language::getLanguage() == lang_es)
						 ? "Exito!"
						 : "Success";
		}
	}
	pageProperties.sendConfirmation(request, result.c_str());
}

int TZManager::calc_offset(time_t t)
{

	// return difference in **seconds** of the tm_mday, tm_hour, tm_min, tm_sec members.
	long tz_offset_second(time_t t);
	struct tm local = *localtime(&t);
	struct tm utc = *gmtime(&t);
	int diff = ((local.tm_hour - utc.tm_hour) * 3600) + ((local.tm_min - utc.tm_min) * 60) + (local.tm_sec - utc.tm_sec);
	int delta_day = local.tm_mday - utc.tm_mday;
	// If |delta_day| > 1, then end-of-month wrap
	if ((delta_day == 1) || (delta_day < -1))
	{
		diff += 86400; // 24L * 60 * 60;
	}
	else if ((delta_day == -1) || (delta_day > 1))
	{
		diff -= 86400; //24L * 60 * 60;
	}

	ILogger::log(l_info, "Local offset calculated as: %d\r\n", diff);

	return diff;
}

void TZManager::splitTime()
{
	time_t now;
	time(&now);
	if (lastSplitTime != now)
	{
		localtime_r(&now, &timeinfoLocal);
		lastSplitTime = now;
	}
}

time_t TZManager::localAsUTCEpoch()
{
	time(&t_now);
	_timeSet = (t_now > 1546300800);
	if (!_timeSet)
		return 0;

	time_t local = t_now + calc_offset(t_now);

	return local;
}

void TZManager::save()
{

	ILogger::log(l_info, "Saving tzone");

	File tzFile = SPIFFS.open("/tzone", "w");
	if (!tzFile)
	{
		ILogger::log(l_error, "Failed to open tzone file for writing");

		return;
	}

	tzFile.println(zoneIndex);
	tzFile.close();
}

void TZManager::load()
{

	ILogger::log(l_info, "loading tzone file: /tzone");
	File tzFile = SPIFFS.open("/tzone", "r");
	if (!tzFile)
	{
		ILogger::log(l_error, "Failed to open globals file");
		return;
	}

	int newZoneIndex = tzFile.parseInt();

	if ((newZoneIndex < 0) || (newZoneIndex >= zoneCount))
	{
		newZoneIndex = 2;
	}
	zoneIndex = newZoneIndex;

	tzFile.close();
}

int TZManager::timeZoneIndex()
{
	return zoneIndex;
}

int TZManager::yday()
{
	splitTime();
	return timeinfoLocal.tm_yday;
}

void TZManager::format(char *buffer, int bufferLength, time_t t)
{
	struct tm tminfo;
	localtime_r(&t, &tminfo);
	strftime(buffer, bufferLength - 1, "%c", &tminfo);
	buffer[bufferLength - 1] = '\0';
}

void TZManager::formatMDHM(char *buffer, int bufferLength, time_t t)
{
	struct tm tminfo;
	localtime_r(&t, &tminfo);

	if (Language::getLanguage() == lang_en_us)
	{
		strftime(buffer, bufferLength - 1, "%m/%d %H:%M", &tminfo);
	}
	else
	{
		strftime(buffer, bufferLength - 1, "%d/%m %H:%M", &tminfo);
	}

	buffer[bufferLength - 1] = '\0';
}