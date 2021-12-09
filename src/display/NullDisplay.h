#pragma once
#include "Display.h"

class NullDisplay : public Display
{
    public:
    NullDisplay();

protected: 
    virtual void update(int reason, const char* msg);

};