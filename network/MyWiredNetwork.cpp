#include "MyWiredNetwork.h"

bool MyWiredNetwork::eth_connected = false;
int MyWiredNetwork::disconnects = 0;

MyWiredNetwork::MyWiredNetwork() {}

void MyWiredNetwork::waitForConnection()
{

	WiFi.onEvent(NetworkEvent);

	// Bus::broadcast(NetworkStatus, "Connecting...");
	ETH.begin();
	ILogger::log(l_info, "Connecting to Ethernet");
	notify( state, "Connecting to Ethernet");

	while (!eth_connected)
	{
		ILogger::log(l_diagnostics, ".");
		delay(5000);
	}
	notify( state, "Ethernet connected");
	notify(IP, ETH.localIP().toString().c_str());
	setMac(ETH.macAddress().c_str());
}

void MyWiredNetwork::NetworkEvent(WiFiEvent_t event)
{
	switch (event)
	{
	case SYSTEM_EVENT_ETH_START:
		ILogger::log(l_info, "ETH Started");
		
		// Bus::broadcast(NetworkStatus, "ETH Started");
		//set eth hostname here
		ETH.setHostname("MyHome");
		break;

	case SYSTEM_EVENT_ETH_CONNECTED:
		ILogger::log(l_info, "ETH Connected");
		// Bus::broadcast(NetworkStatus, "ETH Connected");
		break;

	case SYSTEM_EVENT_ETH_GOT_IP:
		ILogger::log(l_info, "ETH MAC: %s, IPv4: %s\r\r", ETH.macAddress().c_str(), ETH.localIP().toString().c_str());
		// Bus::broadcast(NetworkStatus, ETH.localIP().toString().c_str(), 1);

		ILogger::log(l_info, "Duplex: %s, Speed %d Mbps\r\n", ETH.fullDuplex() ? "Full" : "Half", ETH.linkSpeed());
		eth_connected = true;
		break;

	case SYSTEM_EVENT_ETH_DISCONNECTED:
		ILogger::log(l_warning, "ETH Disconnected");
		// Bus::broadcast(NetworkStatus, "ETH Disconnected");
		eth_connected = false;
		disconnects++;
		if (disconnects > 5)
		{
			restartETH();
		}
		break;

	case SYSTEM_EVENT_ETH_STOP:
		ILogger::log(l_warning, "ETH Stopped");
		// Bus::broadcast(NetworkStatus, "ETH Stopped");
		eth_connected = false;
		break;

	default:
		break;
	}
}

void MyWiredNetwork::restartETH()
{
#ifdef ARDUINO_ESP32_GATEWAY
	digitalWrite(5, LOW);
	delay(3000);
	ETH.begin();
#endif
}