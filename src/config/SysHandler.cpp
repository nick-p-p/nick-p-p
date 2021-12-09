#include "SysHandler.h"

SysHandler::SysHandler() : PageHandler(307)
{
}

void SysHandler::begin()
{
    MyServer::subscribe(this);
}


void SysHandler::webPageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(8000);

    if (!request->hasArg("div"))
    {
        pageProperties.sendMissingArgs(request, "div");
        return;
    }

    String uiSection = request->arg("div");

    if (uiSection == "buildinfo") // version and date on system menu
    {
        char buildTime[31];
        TZManager::format(buildTime, 30, COMPILE_TIME_EPOCH);

        pageProperties.add("sysver", FW_VERSION);
        pageProperties.add("compdate", buildTime);
        pageProperties.send(request);
    }
    else if (uiSection == "preferences")
    {
        pageProperties.add("tempunits", (int)Globals::useCelsius());
        pageProperties.add("msgdebug", Globals::messageDebug());
        pageProperties.send(request);
    }
    else if (uiSection == "webrootdownload") // files for file download page
    {
        buildFileDownloads(request, "/web/", "webrootcontent", pageProperties);
    }
    else if (uiSection == "webendownload") // files for file download page
    {
        buildFileDownloads(request, "/web/en/", "webencontent", pageProperties);
    }
    else if (uiSection == "webesdownload") // files for file download page
    {
        buildFileDownloads(request, "/web/es/", "webescontent", pageProperties);
    }
    else if (uiSection == "datadownload") // files for file download page
    {
        buildFileDownloads(request, "/", "datacontent", pageProperties);
    }
    else
    {
        pageProperties.sendConfirmation(request, Language::phrase(PHRASE_BAD_ARGS));
    }
}

void SysHandler::servePageAction(AsyncWebServerRequest *request, String action)
{
    PageProperties pageProperties(128);
    if (action == "download")
    {
        if (!request->hasArg("file"))
        {
            pageProperties.sendMissingArgs(request, "file");
            return;
        }

        String fileName = request->arg("file");

        if (SPIFFS.exists(fileName))
        {
            //Send file as text
            request->send(SPIFFS, fileName, "text/plain", true);
            // AsyncWebServerResponse *response = request->beginResponse(SPIFFS, fileName, "text/plain");
            // String destHeader = fileName;
            // if (destHeader.startsWith("/"))
            // {
            //     destHeader = destHeader.substring(1);
            // }
            // destHeader = "attachment; filename=" + destHeader;
            // response->addHeader("Content-Disposition", destHeader);
            // request->send(response);
        }

        else
        {
            pageProperties.sendConfirmation(request, Language::phrase(INT_PHRASE_PHRASE_NOT_FOUND));
        }
    }
    else if (action == "restart")
    {
        MyServer::setRestart();
        pageProperties.sendConfirmation(request, "Restarting");
    }
    else
    {
        pageProperties.sendConfirmation(request, "Invalid action");
    }
}

void SysHandler::savePageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(300);
    if (!request->hasArg("tempunits"))
    {
        pageProperties.sendMissingArgs(request, "tempunits");
        return;
    }

    if (!request->hasArg("msgdebug"))
    {
        pageProperties.sendMissingArgs(request, "msgdebug");
        return;
    }

    Globals::setUseCelsius(request->arg("tempunits") != "0");
    Globals::setMessageDebug(request->arg("msgdebug").toInt());
    pageProperties.sendConfirmation(request, Language::phrase(PHRASE_SAVED_SUCCESS));
}

void SysHandler::buildFileDownloads(AsyncWebServerRequest *request, String directory, const char *target, PageProperties &pageProperties)
{
    StringBuilder files(4096);
    bool filesFound = false;

    files.append("<h3>Folder %s</h3><br>", directory.substring(0, directory.length() - 1));

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file)
    {
        if (!file.isDirectory())
        {
            String name = String(file.name());

            if (isInDirectory(name, directory))
            {
                filesFound = true;
                int lastSlash = name.lastIndexOf('/');
                String termName = (lastSlash > 0) ? name.substring(lastSlash + 1) : name;
                files.append("<form onsubmit='event.preventDefault(); return download(\\\"%s\\\");'>", name.c_str())
                    .append("<button>%s</button>", termName.c_str())
                    .append("</form><br>");
            }
        }

        file = root.openNextFile();
    }

    if (filesFound)
    {
        pageProperties.add(target, files.data());
    }

    pageProperties.send(request);
}

bool SysHandler::isInDirectory(String file, String directory)
{
    return (file.startsWith(directory) && (file.lastIndexOf('/') <= directory.length()));
}