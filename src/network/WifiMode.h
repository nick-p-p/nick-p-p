#pragma once
enum WifiConnectionMode {hotspot, connected, connectedWithFallback};

class WifiMode
{
    public:
        static char * WifiModeString(WifiConnectionMode mode);
};
