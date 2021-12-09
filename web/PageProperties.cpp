#include "PageProperties.h"

PageProperties::PageProperties(int capacity)
{

    this->capacity = capacity;
    buffer = new char[capacity + 1];
    buffer[capacity] = '\0';
    used = 0;
    appendChar('[');
}

PageProperties::~PageProperties()
{
    delete[] buffer;
}

void PageProperties::add(const char *name, const char *value)
{

    // put a comma if this is not the first entry
    if (used > 3)
    {
        appendChar(',');
    }

    char *dest = &buffer[used];

    // print {"name":"
    int copied = snprintf(dest, capacity - used, "{\"%s\":\"%s\"}", name, value);
    used += copied;
}

void PageProperties::add(const char *name, int value)
{

    // put a comma if this is not the first entry
    if (used > 3)
    {
        appendChar(',');
    }

    char *dest = &buffer[used];

    // print {"name":"
    int copied = snprintf(dest, capacity - used, "{\"%s\":\"%d\"}", name, value);
    used += copied;
}

void PageProperties::add(const char *name, float value, int dp)
{
    char buffer[12];
    sprintf(buffer, "%.*f", dp, value);
    add(name, buffer);
}

void PageProperties::send(AsyncWebServerRequest *request)
{
    appendChar(']');
    request->send(200, "text/json", buffer);
    if (used < (capacity / 2))
    {
        ILogger::log(l_diagnostics, "Warning, PageProperties capacity of %d is not a good match to using %d\r\n", capacity, used);
    } 
    else if (used > (capacity * 0.9))
    {
        if (used > capacity)
        {
            ILogger::log(l_error, "ERROR!, PageProperties capacity of %d exceeded, using %d\r\n", capacity, used);
        }
        else
        {
            ILogger::log(l_warning, "Warning, PageProperties capacity of %d used more than 90% by %d\r\n", capacity, used);
        }
    }
}

void PageProperties::appendChar(char c)
{
    buffer[used] = c;
    used++;
    buffer[used] = '\0';
}

void PageProperties::sendConfirmation(AsyncWebServerRequest *request, const char *msg)
{
    used = 0;
    appendChar('[');
    add("confirmation", msg);
    send(request);
}

void PageProperties::sendMissingArgs(AsyncWebServerRequest *request, String arg)
{
    used = 0;
    String msg = Language::phrase(INT_PHRASE_ARGS_MISSING) + arg;
    // add("confirmation", msg.c_str());
    request->send(400, "text/plain", msg);
    if ((used < capacity / 2) || (used > capacity * 0.9))
    {
        ILogger::log(l_warning, "Warning, PageProperties capacity of %d is not a good match to using %d\r\n", capacity, used);
    }
}