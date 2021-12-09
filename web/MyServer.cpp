#include "MyServer.h"

const char WEB_UPLOAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
  <title>LEDs</title>
</head>

<body>

  <body>
    <div style='text-align:left;display:inline-block;min-width:260px;'>

      <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>

      <script>
        function setIfEx(name, value) {
          var myEle = document.getElementById(name);
          if (myEle) {
            if (myEle.tagName === 'SELECT' || myEle.tagName === 'INPUT') {
              if (myEle.type == 'checkbox') {
                myEle.checked = (value == "true");
              }
              else {
                myEle.value = value;
              }
            }
            else {
              myEle.innerHTML = value;
            }
          }
        }
      </script>
      <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
        <h1>Upload Web file</h1>
        <br>
        Use this page to upload new web files to the server.
        <br>
        <br>
        Target folder:
        <!-- note the values need to be the full path except the filename ie. end in / -->
        <select id='folder' name='folder'>
          <option value="/web/">root (/web)</option>
          <option value="/web/en/">English (/web/en)</option>
          <option value="/web/es/">Espanol (/web/es)</option>
        </select>

        <br>
        <br>
        <input type='file' name='uploadspiffs'>

        <input type='submit' value='Upload'>
        <br>
        <br>
        <div id=confirmation></div>

      </form>

      <div id='prg'>progress: 0%</div>
      <script>
        $('form').submit(function (e) {
          e.preventDefault();
          var form = $('#upload_form')[0];
          var data = new FormData(form);
          setIfEx('confirmation', '');
          $.ajax({
            url: '/uploadspiffs',
            type: 'POST',
            data: data,
            contentType: false,
            processData: false,
            xhr: function () {
              var xhr = new window.XMLHttpRequest();
              xhr.upload.addEventListener('progress', function (evt) {
                if (evt.lengthComputable) {
                  var per = evt.loaded / evt.total;
                  $('#prg').html('progress: ' + Math.round(per * 100) + '%');
                }
              }, false);
              return xhr;
            },
            success: function (result, status, xhr) {
              setIfEx('confirmation', result);
              console.log('success!')
            },
            error: function (xhr, status, error) {
              setIfEx('confirmation', 'Error: ' + error);
            }
          });
        });
      </script>

      <br />
      <br />
      <br />
      <form action="/upload.htm" method="get">
        <button>Firmware</button>
      </form>

    </div>
  </body>
</body>

</html>
)rawliteral";

const char UPLOAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
    <title>LEDs</title>
  </head>
  <body>
    <div style='text-align:left;display:inline-block;min-width:260px;'>

      <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
      <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
        <h1>Upload Firmware</h1>
        Use this page to upload new firmware to the ESP32.
        <input type='file' name='update' accept='.bin'>
        <input type='submit' value='Update'>
      </form>
      
      <div id='prg'>progress: 0%</div>
      <script>
          $('form').submit(function(e)
          {
            e.preventDefault();
            var form = $('#upload_form')[0];
            var data = new FormData(form);
            $.ajax({
              url: '/update',
              type: 'POST',
              data: data,
              contentType: false,
              processData:false,
              xhr: function() {
                var xhr = new window.XMLHttpRequest();
                xhr.upload.addEventListener('progress', function(evt) {
                  if (evt.lengthComputable) {
                    var per = evt.loaded / evt.total;
                    $('#prg').html('progress: ' + Math.round(per*100) + '%');
                  }
                }, false);
                return xhr;
              },
              success:function(d, s) {
                console.log('success!') 
              },
              error: function (a, b, c) {}
            });
          });
      </script>

      <br/>
      <br/>
      <br/>
      <form action="/webupload.htm" method="get">
        <button>Web files</button>
      </form>
                
    </div>
  </body>
</html>


)rawliteral";

AsyncWebServer* MyServer::server = nullptr; //(80);

//Stream *MyServer::debug;

std::vector<PageHandler *> MyServer::subscribers{};

File MyServer::fsUploadFile;
String MyServer::spiffsName;

bool MyServer::shouldReboot = false;
bool MyServer::SPIFFSUploadStatus = false;

MyServer::MyServer()
{
}

void MyServer::start(bool safeMode, String hostname, int port)//, Stream *debug)
{

  server = new AsyncWebServer(port);

  //this->debug = debug;

  ILogger::log(l_info, "Setting up web server");


  if (!MDNS.begin(hostname.c_str()))
  {
    ILogger::log(l_error, "Error setting up MDNS responder!");
    // debug->println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }

  ILogger::log(l_info, "mDNS responder started");

  if (!safeMode)
  {

    // default page
    server->on("/", handleRoot);

    setupStaticFiles();

    server->on("/page", handlePage);
    server->on("/favicon.ico", handleFavIcon);
  }
  else
  {
    server->on("/webupload.htm", serveStaticWebUpload);
    server->on("/upload.htm", serveStaticUpload);
    server->on("/", serveStaticUpload);
  }
  /*handling uploading firmware file */
  server->on(
      "/update", HTTP_POST,
      [](AsyncWebServerRequest *request) {
        shouldReboot = !Update.hasError();
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "OK" : "FAIL");
        response->addHeader("Connection", "close");
        request->send(response);
      },
      [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index)
        {
          ILogger::log(l_info, "Update Start: %s", filename.c_str());
          //Update.runAsync(true);
          if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
          {
            Update.printError(Serial);
          }
        }
        if (!Update.hasError())
        {
          if (Update.write(data, len) != len)
          {
            Update.printError(Serial);
          }
        }
        if (final)
        {
          if (Update.end(true))
          {
            ILogger::log(l_info, "Update Success: %d", index + len);
          }
          else
          {
            Update.printError(Serial);
          }
        }
      });

  server->on(
      "/uploadspiffs", HTTP_POST, // if the client posts to the upload page
      [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (SPIFFSUploadStatus) ? "Success!" : "Failed!");
        response->addHeader("Connection", "close");
        request->send(response);
      },                 // Send status 200 (OK) to tell the client we are ready to receive
      handleUploadSPIFFS // Receive and save the file
  );

  server->onNotFound(handleNotFound);


  // server.onNotFound(handleNotFound);
  server->begin();
  ILogger::log(l_info, "Web server started");

  MDNS.addService("_http", "_tcp", 80);
  ILogger::log(l_info, "webserver setup complete");
}

void MyServer::setupStaticFiles()
{

  File root = SPIFFS.open("/web");

  File file = root.openNextFile();

  while (file)
  {
    String filename = String(file.name());
    if (filename.startsWith("/web") &&
        (filename.endsWith(".htm") || filename.endsWith(".js") || filename.endsWith(".css")))
    {
      //calc url without /web
      String url = filename;
      url.remove(0, 4);
      ILogger::log(l_info, "Adding web server mapping for %s -> %s\r\n", url.c_str(), filename.c_str());

      server->serveStatic(url.c_str(), SPIFFS, filename.c_str());
    }

    file = root.openNextFile();
  }

  file.close();
}

void MyServer::subscribe(PageHandler *pageHandler)
{
  subscribers.push_back(pageHandler);
  ILogger::log(l_info, "Subscribed web handler with id: %d\r\n", pageHandler->uID());
}

bool MyServer::restart() { return shouldReboot; }

void MyServer::setRestart() { shouldReboot = true; }

AsyncWebServer *MyServer::WebServer() { return server; }

void MyServer::handleRoot(AsyncWebServerRequest *request)
{
  if (Language::getLanguage() == lang_es)
  {
    request->send(SPIFFS, "/web/es/index.htm");
  }
  else
  {
    request->send(SPIFFS, "/web/en/index.htm");
  }
}

void MyServer::handleNotFound(AsyncWebServerRequest *request)
{
  String path = request->url();
  if (SPIFFS.exists(path))
  {
    request->send(SPIFFS, path.c_str());
  }
  request->send(404, "text/plain", path.c_str());
}

void MyServer::serveStaticWebUpload(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", WEB_UPLOAD);
}

void MyServer::serveStaticUpload(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", UPLOAD);
}

void MyServer::handlePage(AsyncWebServerRequest *request)
{
  if (!request->hasArg("UID"))
  {
    ILogger::log(l_error, "Error incoming page request has no UID: %s\r\n", request->url().c_str());
    request->send(400, "text/plain", request->url());
    return;
  }

  int UID = request->arg("UID").toInt();

  for (PageHandler *pageHandler : subscribers)
  {
    if (pageHandler->uID() == UID)
    {
      pageHandler->handleRequest(request);
      return;
    }
  }

  ILogger::log(l_error, "Error incoming page request no handler for UID: %d for %s\r\n", UID, request->url().c_str());
  request->send(404, "text/plain", request->url());
}

void MyServer::handleFavIcon(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/web/favicon-32x32.png", "image/png");
}

void MyServer::firmwareUploadPage()
{
}

void MyServer::handleUploadSPIFFS(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    ILogger::log(l_info, "UploadStart: %s\r\n", filename.c_str());

    SPIFFSUploadStatus = false;
    if (request->hasArg("folder"))
    {
      spiffsName = request->arg("folder") + filename;
    }
    else
    {
      request->send(400, "text/plain", "no folder");
    }

    if (!spiffsName.startsWith("/"))
    {
      spiffsName = "/" + spiffsName;
    }
    ILogger::log(l_info, "SPIFFS upload Name: %s\r\n", spiffsName.c_str());
    // open the file on first call and store the file handle in the request object
    request->_tempFile = SPIFFS.open(spiffsName, FILE_WRITE);
    spiffsName = String();
  }

  if (len)
  {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
  }
  if (final)
  {
    ILogger::log(l_info, "UploadEnd: %s, %d\r\n", filename.c_str(), index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();

    SPIFFSUploadStatus = true;
    request->send(200, "text/plain", "File Uploaded !");
    int tBytes = SPIFFS.totalBytes(); 
    int uBytes = SPIFFS.usedBytes();
    ILogger::log(l_diagnostics, "SPIFFS total %d, used %d, free %d\r\n", tBytes, uBytes, tBytes-uBytes);
  }
}

bool MyServer::sendFileOrError(AsyncWebServerRequest *request, const char *fileName)
{
  if (SPIFFS.exists(fileName))
  {

    if (String(fileName).endsWith(".css"))
    {
      request->send(SPIFFS, fileName, "text/css");
    }
    else if (String(fileName).endsWith(".js"))
    {
      request->send(SPIFFS, fileName, "text/javascript");
    }
    else
    {
      request->send(SPIFFS, fileName, "text/html");
    }
    return true;
  }
  else
  {
    ILogger::log(l_error, "streaming %s, not found\r\n", fileName);
    request->send(404, "text/plain", fileName);
  }
  return false;
}

void MyServer::sendError(AsyncWebServerRequest *request, int code, const char *msg)
{
  AsyncWebServerResponse *response = request->beginResponse(code, "text/plain", msg);
  request->send(response);
}
