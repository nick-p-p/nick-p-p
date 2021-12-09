#pragma once
#include <TZManager.h>
#include <PageHandler.h>
#include <PageProperties.h>
#include <StringBuilder.h>
#include <Language.h>

#include "Globals.h"
// #include "core/logging/DebugStream.h"


class  SysHandler : public PageHandler
{
public:
    SysHandler();

    void begin();

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


    /**
     * @brief optional, handle actions other than above
     */ 
    virtual void servePageAction(AsyncWebServerRequest *request, String action);

// -------------------------------------------------------------------------------


private:
    void buildFileDownloads(AsyncWebServerRequest *request, String directory, const char* target, PageProperties &pageProperties);

    /**
     * @brief return true if the file is in the given directory and no a subdirectory
     * 
     * @param file - full name includign path of file.
     * @param directory - full name of directory including trailing slash e.g. \web\
     */ 
    bool isInDirectory(String file, String directory);
};