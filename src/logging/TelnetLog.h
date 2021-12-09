#pragma once
#include "ILogger.h"
#include <Arduino.h>

#include <WiFi.h>

class TelnetLog : public ILogger
{
public:
    TelnetLog();
    /**
     * initializee the networking so telnet clients can connect
     */
    void startNetworking();

protected:
    virtual void logInst(const char *msg, uint8_t flags);

private:
    boolean disconnected();


    WiFiServer server;
    WiFiClient client;
};