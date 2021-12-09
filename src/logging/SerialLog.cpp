#include "SerialLog.h"

SerialLog::SerialLog(Stream * stream)
{
    this->stream = stream;
    subscribe(this);
    log(l_info, "Serial log started.");
}

void SerialLog::logInst(const char *msg, uint8_t flags)
{
    stream->println(msg);
}

