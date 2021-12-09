#include "WifiMode.h"



char * WifiMode::WifiModeString(WifiConnectionMode mode)
{
    switch(mode)
    {
        case hotspot:
            return "hotspot";
            break;

        case connected:
            return "connected";
            break;

        case connectedWithFallback:
            return "connected with fallback";
            break;


    }
}

