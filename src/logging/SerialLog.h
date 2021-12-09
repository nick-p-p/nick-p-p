#pragma once
#include "ILogger.h"
#include <Arduino.h>

class SerialLog : public ILogger
{
public:
    SerialLog(Stream * stream);

protected:
    virtual void logInst(const char *msg, uint8_t flags);    



private:
    Stream * stream = nullptr;    

};
