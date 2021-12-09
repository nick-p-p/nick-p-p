// #include "core/config/BoardsConfig.h"

#include "MyWifiNetwork.h"

DNSServer MyWifiNetwork::dns;
MyWifiNetwork *MyWifiNetwork::instance = nullptr;

MyWifiNetwork::MyWifiNetwork(String dnsName)//, WifiConnectionMode mode)
{
	instance = this;

	// this->mode = mode;
	this->dnsName = dnsName;
}

void MyWifiNetwork::loop( )
{
	checkConnection();
}


void MyWifiNetwork::checkConnection()
{
	unsigned long currentMillis = millis();
	// if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
	if ((currentMillis - previousCheckMillis >= checkIntervalMillis) && (WiFi.status() != WL_CONNECTED))
	{
		WiFi.disconnect();
		WiFi.reconnect();
		ILogger::log(l_error,"Wifif network lost, reconnection: %s", (WiFi.status() == WL_CONNECTED) ? "success" : "failed" );
		previousCheckMillis = currentMillis;
	}
}

// void MyWifiNetwork::setMode( WifiConnectionMode mode)
// {
// 	this->mode = mode;
// }

void MyWifiNetwork::waitForConnection()
{
	WifiConnectionMode mode = WifiHandler::getWifiMode();
	// Bus::broadcast(NetworkStatus, "Starting wifi");
	ILogger::log(l_info, "Starting wifi, in mode: ");
	ILogger::log(l_info, mode == connected ? "Connected" : mode == connectedWithFallback ? "Connected With Fallback" : "Hotspot");
	notify(state, "Starting wifi");

	if (mode == connected || mode == connectedWithFallback)
	{
		AsyncWebServer wifiServer(80);

		AsyncWiFiManager wifiManager(&wifiServer, &dns);

	
		wifiManager.setAPCallback(configModeCallback);
		wifiManager.autoConnect(dnsName.c_str());
		notify(state, "WiFi connected");
		ILogger::log(l_info, "wifi connected at %s\r\n", WiFi.localIP().toString().c_str());
		notify(IP, WiFi.localIP().toString().c_str());
		notify(SSID, WiFi.SSID().c_str());
#ifdef WIFI_HOTSPOT_ACTIVE_PIN
		Serial.println("WIFI_HOTSPOT_ACTIVE_PIN off");
		digitalWrite(WIFI_HOTSPOT_ACTIVE_PIN, LOW);
#endif
	}
	else
	{
#ifdef WIFI_HOTSPOT_ACTIVE_PIN
		Serial.println("WIFI_HOTSPOT_ACTIVE_PIN on");
		digitalWrite(WIFI_HOTSPOT_ACTIVE_PIN, HIGH);
#endif
		WiFi.mode(WIFI_AP);
		WiFi.softAP(dnsName.c_str(), NULL, 1, 0, 1);
		delay(2000);
		WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0)); // subnet FF FF FF 00

		// Start the access point
		ILogger::log(l_info, "Access Point %s started, IP address: 192.168.4.1", WiFi.SSID().c_str());

		notify(state, "WiFi hotspot");
		notify(IP, "192.168.4.1");
		notify(SSID, dnsName.c_str());
	}

	setMac(WiFi.macAddress().c_str());


	// Bus::broadcast(NetworkStatus, WiFi.localIP().toString().c_str(), SUBTYPE_IP_ADDRESS);
	// esp_wifi_set_ps(WIFI_PS_NONE);
	ILogger::log(l_debug, "Starting Wifi handler");
	wifiHandler.begin();
}

void MyWifiNetwork::configModeCallback(AsyncWiFiManager *myWiFiManager)
{
#ifdef WIFI_HOTSPOT_ACTIVE_PIN
	Serial.println("WIFI_HOTSPOT_ACTIVE_PIN on");
	digitalWrite(WIFI_HOTSPOT_ACTIVE_PIN, HIGH);
#endif
	instance->notify(state, "Access point");
	instance->notify(IP, WiFi.softAPIP().toString().c_str());
	instance->notify(SSID, WiFi.SSID().c_str());
}
