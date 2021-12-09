#include "TimeZoneEntry.h"

TimeZoneEntry::TimeZoneEntry(const char *description, const char *timezone)
{
    this->description = description;
    this->timezone = timezone;
}

const char *TimeZoneEntry::Description()
{
    return description;
}

const char *TimeZoneEntry::Timezone()
{
    return timezone;
}
