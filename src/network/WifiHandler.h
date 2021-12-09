#pragma once
#include "WifiMode.h"
#include <PageHandler.h>
#include <PageProperties.h>
#include <StringBuilder.h>
#include <Language.h>
#include <ArduinoJson.h>


class  WifiHandler : public PageHandler
{
public:
    WifiHandler();

    void begin();

    static WifiConnectionMode getWifiMode();

    // static void setWifiMode(WifiConnectionMode mode);
/**
 *     SECTION FOR PAGEHANDLER OVERIDES
 */

    /**
     * @brief optional, return new layout to replace the file contents
     * called when action = "layout"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void webPageData(AsyncWebServerRequest *request);

    /**
     * @brief optional, save data sent from the page
     * called when action = "save"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void savePageData(AsyncWebServerRequest *request);


// -------------------------------------------------------------------------------

private:
    void save();
    void load();

    static WifiConnectionMode wifiMode;

    const char filename[15] = "/wifimode.json";

};