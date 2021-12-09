#pragma once
#include "Arduino.h"
#include <AsyncTCP.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include <Language.h>
#include <PageHandler.h>
#include <ILogger.h>
#include <vector>

class MyServer
{
public:
    /**
     * @brief constructor
     */
    MyServer();


    /**
     * @brief start the web server, must be after netwroking connected
     * @param safeMode - if true the server only performs code uploads
     */
    void start(bool safeMode, String hostname, int port = 80);//, Stream * debug);

    /**
     * subscribe a sub system so it can receive incoming requests
     */
    static void subscribe(PageHandler *pageHandler);

    /**
     * web server on port 80
     */
    static AsyncWebServer* server;

    /**
     * indicates that the ESP should restart
     */
    static bool restart();


    /**
     * set the restart flag to true
     */ 
    static void setRestart();

    /**
     * @breif pointer to the webserver
     */ 
    static AsyncWebServer * WebServer();

    static void initWebServerForWifiManager();

private:
    /**
     * all the subscribed handlers
     */
    static std::vector<PageHandler *> subscribers;

    // static PageBuilder pageBuilder;

    /**
     * setup handlers for all htm, js and css files in the SPIFS /web folder and below
     */
    void setupStaticFiles();

    /**
     * default page / root page
     */
    static void handleRoot(AsyncWebServerRequest *request);

    /**
     * non static pages
     */
    static void handlePage(AsyncWebServerRequest *request);

    static void handleFavIcon(AsyncWebServerRequest *request);

    static void handleNotFound(AsyncWebServerRequest *request);

    static void serveStaticWebUpload(AsyncWebServerRequest *request);

    static void serveStaticUpload(AsyncWebServerRequest *request);

    /**
     * firmware upload menu
     */
    static void firmwareUploadPage();

    /**
     * variable used to handle the state of uploading to SPIFFS
     */
    static File fsUploadFile; // destination filename at source (without path)
    static String spiffsName; // name locally (with path)
    static bool SPIFFSUploadStatus;
    static void handleUploadSPIFFS(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

    /**
     * set by uploading new firmware, if true causes an ESP reset
     */
    static bool shouldReboot;

    /**
     * @brief stream the file from SPIFFS or send a 404
     * @param fileName
     */
    static bool sendFileOrError(AsyncWebServerRequest *request, const char *fileName);

    static void sendError(AsyncWebServerRequest *request, int code, const char *msg);

   // static Stream * debug;
};
