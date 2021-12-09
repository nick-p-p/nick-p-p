#include "MyNetwork.h"

MyServer MyNetwork::server;
char MyNetwork::macId[SYSTEM_ID_LEN + 1];

MyNetwork::MyNetwork()
{
}

void MyNetwork::begin(bool safeMode, String hostname, int port)
{
	waitForConnection();
	ILogger::log(l_info, "Start web server");
	server.start(safeMode, hostname, port);
}

void MyNetwork::loop(){}

void MyNetwork::setMessageCallback(void (*func)(NetMessage, const char *))
{
	_messsgeCallback = func;
}

void MyNetwork::setMac(const char *mac)
{
	int macPos = 0;
	int destPos = 0;
	char c;
	while (((c = mac[macPos]) != '\0') && (destPos < SYSTEM_ID_LEN))
	{
		macPos++;
		if (c != ':')
		{
			macId[destPos] = c;
			destPos++;
		}
	}
	macId[destPos] = '\0';
}

char *MyNetwork::MacId()
{
	return &macId[0];
}

void MyNetwork::notify(NetMessage netMessage, const char *msg)
{
	if (_messsgeCallback != NULL)
	{
		_messsgeCallback(netMessage, msg);
	}
}