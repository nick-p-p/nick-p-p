#include "TelnetLog.h"

TelnetLog::TelnetLog()  : server(23)
{
    subscribe(this);
}

void TelnetLog::startNetworking()
{
    server.begin();
    client = server.available();
    log(l_info, "Telnet log started.");
}

void TelnetLog::logInst(const char *msg, uint8_t flags)
{
    if (!disconnected())
    {
        client.write(msg, strlen(msg));
        client.write('\r');
        client.write('\n');
    }
}

boolean TelnetLog::disconnected()
{
    if (!server)
        return true;

    if (!client)
    {
        client = server.available();
    }
    if (client)
    {
        if (client.connected())
            return false;
        client.stop();
        client = server.available();
    }
    return true;
}