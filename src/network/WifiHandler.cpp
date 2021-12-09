#include "WifiHandler.h"

WifiConnectionMode WifiHandler::wifiMode = connected;

WifiHandler::WifiHandler() : PageHandler(315)
{
}

void WifiHandler::begin()
{
    load();
    MyServer::subscribe(this);
}

WifiConnectionMode WifiHandler::getWifiMode(){ return wifiMode;}

void WifiHandler::webPageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(8000);

    if (!request->hasArg("div"))
    {
        pageProperties.sendMissingArgs(request, "div");
        return;
    }

    String uiSection = request->arg("div");

    if (uiSection == "wifi")
    {
        pageProperties.add("wifiMode", (int)wifiMode);
        pageProperties.send(request);
    }
    else
    {
        pageProperties.sendConfirmation(request, Language::phrase(PHRASE_BAD_ARGS));
    }
}

// void SysHandler::servePageAction(AsyncWebServerRequest *request, String action)
// {
//     PageProperties pageProperties(128);
//     if (action == "download")
//     {
//         if (!request->hasArg("file"))
//         {
//             pageProperties.sendMissingArgs(request, "file");
//             return;
//         }

//         String fileName = request->arg("file");

//         if (SPIFFS.exists(fileName))
//         {
//             //Send file as text
//             request->send(SPIFFS, fileName, "text/plain", true);
//             // AsyncWebServerResponse *response = request->beginResponse(SPIFFS, fileName, "text/plain");
//             // String destHeader = fileName;
//             // if (destHeader.startsWith("/"))
//             // {
//             //     destHeader = destHeader.substring(1);
//             // }
//             // destHeader = "attachment; filename=" + destHeader;
//             // response->addHeader("Content-Disposition", destHeader);
//             // request->send(response);
//         }

//         else
//         {
//             pageProperties.sendConfirmation(request, Language::phrase(INT_PHRASE_PHRASE_NOT_FOUND));
//         }
//     }
//     else if (action == "restart")
//     {
//         MyServer::setRestart();
//         pageProperties.sendConfirmation(request, "Restarting");
//     }
//     else
//     {
//         pageProperties.sendConfirmation(request, "Invalid action");
//     }
// }

void WifiHandler::savePageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(300);
    if (!request->hasArg("wifiMode"))
    {
        pageProperties.sendMissingArgs(request, "wifiMode");
        return;
    }

    int newMode = request->arg("wifiMode").toInt();

    WifiConnectionMode newWifiMode = (WifiConnectionMode)request->arg("wifiMode").toInt();

    if( newWifiMode != wifiMode )
    {
        pageProperties.sendConfirmation(request, Language::phrase(PHRASE_SAVED_SUCCESS));  
        MyServer::setRestart();
        save();
    }
    else
    {
        pageProperties.sendConfirmation(request, Language::phrase(PHRASE_NO_CHANGES));  
    }
    
}

void WifiHandler::save()
{
    File settingsFile = SPIFFS.open(filename, "w");
    if (!settingsFile)
    {
        ILogger::log(l_error, "Failed to open wifi settings file for writing: ", filename);
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<128> doc;

    // Set the values in the document
    doc["wifiMode"] = wifiMode;

    // Serialize JSON to file
    if (serializeJson(doc, settingsFile) == 0)
    {
        ILogger::log(l_debug, "Failed to write to file for wifi settings ");
    }

    settingsFile.close();

    ILogger::log(l_debug, "Wifi mode  saved: %s", WifiMode::WifiModeString(wifiMode));
    
}

void WifiHandler::load()
{
    wifiMode = connected;

    ILogger::log(l_debug, "loading wifi settings");
    File settingsFile = SPIFFS.open(filename, "r");
    if (!settingsFile)
    {
        ILogger::log(l_error, "Failed to open wifi settings file %s", filename);
        
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<128> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, filename);
    if (error)
    {
        ILogger::log(l_error, "Failed to read ifi settings file %s, using default configuration.", filename);
        return;
    }

    // Copy values from the JsonDocument to the Config
    // Set the values in the document
    wifiMode = doc["wifiMode"].as<WifiConnectionMode>();

    settingsFile.close();
    ILogger::log(l_debug, "Wifi mode  loaded: %s", WifiMode::WifiModeString(wifiMode));

}