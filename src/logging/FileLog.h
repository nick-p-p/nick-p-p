#pragma once
#include <FileLogEntry.h>
#include <ILogger.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <StringBuilder.h>
#include    <Language.h>
//N.B. cannot use TZMAnager as circular dependency

class FileLog : public ILogger
{
public:
    /**
     * @brief constructor
     * @param filename - the SPIFFS filename to persist to e.g. "/syslog.txt"
     * @param entries - how nany records to keep in memory
     * @param daysToRetain - records older than this are not saved
     */
    FileLog(const char *fileName, int entries, int daysToRetain);

    /**
     * @brief initialize the log, load from disk
     */
    bool begin();

    /**
     * @brief save the log to filesystem 
     */
    bool saveToDisk();

    /**
     * @brief true if there are no entries
     */ 
    bool isEmpty();
    
    /**
     * @brief access to the raw content
     */ 
    FileLogEntry * rawLines();

    /**
     * @brief count of active lines
     */ 
    int lineCount();

    /**
     * @brief wipe out the content
     */
    void clear(); 

    void appendWebPageData(StringBuilder & stringBuilder);

protected:
    virtual void logInst(const char *msg, uint8_t flags);    

private:
    /**
     * @brief local copy of time as using TZMAnager would create a circular reference
     */ 
    time_t now();
    
    /**
     * @brief add to the log in memory
     * 
     */
    void logWithTime(const char *message, time_t logTime);

    /**
     * @brief load from disk
     */
    bool load();
    
    /**
     * @brief save the log to filesystem 
     */
    bool save();  

     /**
    * @brief true if the log has reached capacity
    */ 
    bool full();

    /**
     * @brief move all entries down one to purge out the oldest
     */
    void shuffledown();



    const char *fileName = nullptr;

    /**
     * how many lines in memory, set in ctor
     */ 
    int numberOfLines = 0;

    /**
     * purge records older than this when saving to disk, set in ctor
     */ 
    int daysToRetain = 20;

    /**
     * pointer to the in memory data
     */ 
    FileLogEntry * lines = nullptr;
    
    /**
     * how many entries have data in
     */ 
    int usedEntries;


    /**
     * true if entries have been added since last save
     */ 
    bool unSavedLines = false; 

};
