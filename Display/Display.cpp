#include "Display.h"

Display * Display::instance = nullptr;

Display::Display(Display * instancePtr){
    instance = instancePtr;
}

void Display::begin(){}

void Display::inform(int reason, const char* msg)
{
    if (instance != nullptr)
    {
        instance->update(reason, msg);
    }
}


void Display::setActive(bool active){};