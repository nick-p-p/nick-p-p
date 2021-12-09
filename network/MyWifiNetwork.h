#pragma once
#include "MyNetwork.h"

#include "WiFi.h"
#include <ESPAsyncWiFiManager.h> 

#define WIFI_MODE_CONNECTED 0
#define WIFI_MODE_HOTSPOT   1

enum WifiConnectionMode {hotspot, connected, connectedWithFallback};

class MyWifiNetwork : public MyNetwork
{
public:
    MyWifiNetwork(String dnsName, WifiConnectionMode mode);


    virtual void loop();
    
    // void setMode(bool connected);

    void checkConnection();

protected:
    virtual void waitForConnection();
    static void configModeCallback(AsyncWiFiManager *myWiFiManager);
    static DNSServer dns;
    static MyWifiNetwork* instance;
    WifiConnectionMode mode = hotspot;
    // bool connected = false;
    String dnsName = "";

private:
    unsigned long previousCheckMillis = 0;
    unsigned long checkIntervalMillis = 30000;    
};