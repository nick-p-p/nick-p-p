#pragma once
#include "MyServer.h"
#include "Arduino.h"
#include <Language.h>
#include <ILogger.h>


class PageProperties
{

public:
    PageProperties(int capacity);
    ~PageProperties();
    void add (const char* name, const char* value);
    void add (const char* name, int value);
    void add (const char* name, float value, int dp);
    
    void send(AsyncWebServerRequest *request);
    void sendConfirmation(AsyncWebServerRequest *request, const char* msg);
    void sendMissingArgs(AsyncWebServerRequest *request, String arg);

    private:
    
    char * buffer = nullptr;
    int capacity = 0;
    int used = 0;
    void appendChar(char c);
    
    
};


