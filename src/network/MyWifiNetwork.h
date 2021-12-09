#pragma once
#include "MyNetwork.h"
#include "WifiMode.h"
#include "WiFi.h"
#include <ESPAsyncWiFiManager.h> 
#include <WifiHandler.h>


class MyWifiNetwork : public MyNetwork
{
public:
    MyWifiNetwork(String dnsName);//, WifiConnectionMode mode);

    virtual void loop();

    /**
     * @brief call this from time to time to refresh the connection
     */ 
    void checkConnection();

    // void setMode( WifiConnectionMode mode);
protected:
    virtual void waitForConnection();
    static void configModeCallback(AsyncWiFiManager *myWiFiManager);
    static DNSServer dns;
    static MyWifiNetwork* instance;
    // WifiConnectionMode mode = hotspot;

    String dnsName = "";

private:
    unsigned long previousCheckMillis = 0;
    unsigned long checkIntervalMillis = 30000;    
    WifiHandler wifiHandler;
};