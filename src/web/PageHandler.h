#pragma once
#include <ESPAsyncWebServer.h>
#include <ILogger.h>
// #include "PageBuilder.h"
// #include "core/config/GlobalIDs.h"
// #define TIMEZONE_MANAGER_UID    300
// #define SYSLOG_UID              40

/**
 * @brief base class for classes that respond to web requests
 */ 
class PageHandler
{
public:
    PageHandler(int uniqueID);
    
    /**
     * @brief unique ID for this handler. it is used to match against the "UID" parameter
     * of any incoming request
     */ 
    int uID();

    /**
     * @brief respond to the web request, including any errors
     */ 
    virtual void handleRequest(AsyncWebServerRequest *request); //, Stream* debug);

    /**
     * @brief optional, return new layout to replace the file contents
     * called when action = "layout"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void webPageContent(AsyncWebServerRequest *request);

    /**
     * @brief optional, return data to update the page typically for config
     * called when action = "data"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void webPageData(AsyncWebServerRequest *request);

    /**
     * @brief optional, return info to update the page, typically every few seconds for status display
     * called when action = "infodata"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void webPageInfoData(AsyncWebServerRequest *request);


    /**
     * @brief optional, return new content for a specific block
     * called when action = "loadblock"
     * return a string of the new content to replace the current content of the block
     */ 
    virtual void webPageBlock(AsyncWebServerRequest *request);

    /**
     * @brief optional, save data sent from the page
     * called when action = "save"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */ 
    virtual void savePageData(AsyncWebServerRequest *request);

    /**
     * @brief optional, handle actions other than above
     */ 
    virtual void servePageAction(AsyncWebServerRequest *request, String action);

private:
    int uid = 0;    

};