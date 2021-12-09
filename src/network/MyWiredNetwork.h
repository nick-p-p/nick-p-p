#pragma once
#include "MyNetwork.h"
#include <ETH.h>


class MyWiredNetwork : public MyNetwork
{
public:
    MyWiredNetwork();


protected:
    virtual void waitForConnection();

private:

static void NetworkEvent(WiFiEvent_t event);

static bool eth_connected;

static void restartETH();
static int disconnects;

};