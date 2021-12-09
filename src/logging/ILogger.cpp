#include "ILogger.h"

char ILogger::buffer[LOGGER_BUFFER_LENGTH + 1] = "";

logLevel ILogger::thislevel = l_info;

std::mutex ILogger::debugLockObj;

std::vector<ILogger *> ILogger::subscribers{};

ILogger::ILogger()
{
}

void ILogger::log(logLevel level, const char *msg, ...)
{
    if (level <= thislevel)
    {

        debugLockObj.lock();

        va_list argList;
        va_start(argList, msg);
        vsprintf(&buffer[0], msg, argList);
        va_end(argList);
        buffer[LOGGER_BUFFER_LENGTH] = '\0';

        callSubscribers(buffer, 0);

        debugLockObj.unlock();
    }
}

void ILogger::log(logLevel level, uint8_t flags, const char *msg, ...)
{
    if (level <= thislevel || flags != 0)
    {

        debugLockObj.lock();

        va_list argList;
        va_start(argList, msg);
        vsprintf(&buffer[0], msg, argList);
        va_end(argList);
        buffer[LOGGER_BUFFER_LENGTH] = '\0';

        callSubscribers(buffer, flags);

        debugLockObj.unlock();
    }
}

void ILogger::setLevel(logLevel level)
{
    thislevel = level;
    log(l_info, "Log level set to: %d", level );
}

logLevel ILogger::getLevel(){ return thislevel;}

void ILogger::subscribe(ILogger *logger)
{
    subscribers.push_back(logger);
}

void ILogger::callSubscribers(const char *msg, uint8_t flags)
{
    for (ILogger *logger : subscribers)
    {
        logger->logInst(msg, flags);
    }
}