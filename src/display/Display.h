#pragma once
#include <Arduino.h>

enum DisplayReason{ 
        // N.b. these  four must align to MyNetwork::NetMessage
    networkStatus,
    IPaddress,
    SSID,
    WifiMode,

    
    paletteName,
    paletteNameFixed,
    safeMode
};

class Display
{
public:
    Display(Display * instancePtr);

    virtual void begin();

    static void inform(int reason, const char* msg);

    virtual void setActive(bool active);

protected:
    virtual void update(int reason, const char* msg) = 0;

    static Display * instance;

};