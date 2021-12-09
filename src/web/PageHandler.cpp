#include "PageHandler.h"

PageHandler::PageHandler(int uniqueId) {
    uid = uniqueId;
}

int PageHandler::uID()
{
    return uid;
}

void PageHandler::handleRequest(AsyncWebServerRequest *request)
{
    if (!request->hasArg("action"))
    {
        ILogger::log(l_error, "Error incoming page request has no action: %s", request->url().c_str());
        
        request->send(400, "text/plain", request->url());
        return;
    }

    String action = request->arg("action");

    if (action == "infodata")
    {
        webPageInfoData(request);
    }

    else if (action == "layout")
    {
        webPageContent(request);
    }

    else if (action == "data")
    {
        webPageData(request);
    }

    else if (action == "loadblock")
    {
        webPageBlock(request);
    }

    else if (action == "save")
    {
        savePageData(request);
    }
    else
    {
        servePageAction(request, action);
    }
}

void PageHandler::webPageContent(AsyncWebServerRequest *request) {}

void PageHandler::webPageData(AsyncWebServerRequest *request) {}

void PageHandler::savePageData(AsyncWebServerRequest *request) {}

void PageHandler::webPageInfoData(AsyncWebServerRequest *request) {}

void PageHandler::webPageBlock(AsyncWebServerRequest *request) {}

void PageHandler::servePageAction(AsyncWebServerRequest *request, String action)
{
    // server.sendHeader("Content-Length", String(server.uri().length()));
    request->send(404, "text/plain", request->url());
}