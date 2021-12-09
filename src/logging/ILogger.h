#pragma once

#include <mutex>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#define LOGGER_BUFFER_LENGTH 120

#define LOG_TO_FILE 1

enum logLevel {nologging, l_error, l_warning, l_info, l_debug, l_diagnostics};
/**
 * @brief This is an abstract class for logging to various destinations based
 *  on the implementation instantiated
 * 
 *   No output will be created unless a derived object is created that subscribes to messages
 *   pointer
 */ 
class ILogger
{
public:    
    ILogger();

    /**
     * @brief log an output message
     * 
     * @param level - gravity of this message, will be supressed if the current level is lower
     * @param msg - text to outpyt and any substitution parameters
     */ 
    static void log(logLevel level, const char *msg, ...);

   /**
     * @brief log an output message
     * 
     * @param level - gravity of this message, will be supressed if the current level is lower
     * @param msg - text to outpyt and any substitution parameters
     */ 
    static void log(logLevel level, uint8_t flags, const char *msg, ...);

    /**
     * @brief level - set the level above which messages ar suppressed
     */ 
    static void setLevel(logLevel level);

    static logLevel  getLevel();

    void subscribe(ILogger *logger);


protected:
    /**
     * @brief write to actual destination, implementation specific
     */ 
    virtual void logInst(const char *msg, uint8_t flags) = 0;

    // curent level for logging
    static logLevel thislevel;

private:

    static void callSubscribers(const char * msg , uint8_t flags);

    /**
     * buffer for expanding the message
     */ 
    static char buffer[LOGGER_BUFFER_LENGTH+1];

    /**
     * all the subscribed handlers
     */
    static std::vector<ILogger *> subscribers;

#ifdef ARDUINO_ARCH_ESP32    
    /**
     * mutex to ensure thread safety
     */
    static std::mutex debugLockObj;
#endif    
};