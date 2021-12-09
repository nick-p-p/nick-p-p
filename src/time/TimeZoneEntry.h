#pragma once
#include "Arduino.h"

//#include <TimeLib.h>

/*
 Class for tracking and reporting battery voltage
*/
class TimeZoneEntry
{
public:
	/*
		Constructor

	*/
	TimeZoneEntry(const char * description, const char * timezone);

	/*
	 Initializes the timezone loading the selected value and custom entries from file
	*/
	const char * Description();
    const char * Timezone();

private:
	// data
	const char * description;
	const char * timezone;
};
