
#include "FileLog.h"

FileLog::FileLog(const char *fileName, int entries, int daysToRetain)
{
    usedEntries = 0;

    this->fileName = fileName;
    this->numberOfLines = entries;
    this->daysToRetain = daysToRetain;
    // allocate the lines in psram
    int sizeToAllocate = entries * sizeof(FileLogEntry);

    if (psramFound())
    {
        lines = (FileLogEntry *)ps_malloc(sizeToAllocate);
    }
    else
    {
        lines = (FileLogEntry *)malloc(sizeToAllocate);
    }
}

bool FileLog::begin()
{
    bool loaded = load();
    subscribe(this);
    log(l_info, "File log started.");
    return loaded;
}

bool FileLog::saveToDisk()
{
    if (unSavedLines)
    {
        save();
        return true;
    }
    return false;
}


    bool FileLog::isEmpty()
    {
        return usedEntries == 0;
    }
    
    
    FileLogEntry * FileLog::rawLines()
    {
        return lines;
    }

    
    int FileLog::lineCount()
    {
        return usedEntries;
    }

void FileLog::logInst(const char *msg, uint8_t flags)
{
    if (flags == LOG_TO_FILE)
    {
        logWithTime(msg, now());
    }
}

time_t FileLog::now()
{
    time_t t_now;

	time(&t_now);

	return t_now > 1546300800 ? t_now : 0;
}

void FileLog::logWithTime(const char *inMessage, time_t logTime)
{
    if (full())
    {
        shuffledown();
    }

    lines[usedEntries].logTime = logTime;

    strncpy(lines[usedEntries].message, inMessage, MAX_LINE_LENGTH);
    lines[usedEntries].message[MAX_LINE_LENGTH] = '\0';
    usedEntries++;
    unSavedLines = true;
}

bool FileLog::load()
{
    ILogger::log(l_info, "loading: %s", fileName);
    File histFile = SPIFFS.open(fileName, "r");
    if (!histFile)
    {
        ILogger::log(l_error, "Failed to open history file");
        return false;
    }

    size_t size = histFile.size();
    ILogger::log(l_info, "size: %d", size);
    if (size > 8192)
    {
        ILogger::log(l_error, "log file size is too large, max 8k");
        histFile.close();
        if (SPIFFS.remove(fileName))
        {
            ILogger::log(l_info, "%s deleted", fileName);
        }
        return false;
    }

    ILogger::log(l_info, "reading: ");
    usedEntries = 0;
    bool noMoreData = false;
    char buffer[MAX_LINE_LENGTH + 1];
    while (!noMoreData) //histFile.available())
    {

        long logTime = histFile.parseInt();
        if (logTime == 0)
        {
            noMoreData = true;
        }
        else
        {
            byte comma = histFile.read(); // skip comma
            if (comma == -1)
            {
                noMoreData = true;
            }
            else
            {
                bool eol = false;
                int buffPos = 0;
                while (histFile.available() && !eol && buffPos < MAX_LINE_LENGTH)
                {
                    char ch = histFile.read();
                    if (ch == '\n')
                    {
                        eol = true;
                    }
                    else
                    {
                        if (isprint(ch))
                        {
                            buffer[buffPos] = ch;
                            buffPos++;
                        }
                    }
                }
                // int numRead = histFile.readBytesUntil('\n', buffer, MAX_LINE_LENGTH);
                if (buffPos == 0)
                {
                    noMoreData = true;
                }
                else
                {
                    buffer[buffPos] = '\0';

                    if (logTime > 0)
                    {
                        logWithTime(buffer, logTime);
                    }
                }
            }
        }
    }

    ILogger::log(l_info, "loaded log history entries: %d", usedEntries);
    unSavedLines = false;
    histFile.close();
    return true;
}

bool FileLog::save()
{
    if (!unSavedLines)
    {
        return false;
    }

    time_t timeNow = now();
    ILogger::log(l_info, "Saving log history");
    time_t historyLimit = timeNow - (daysToRetain * 60 * 60 * 24);

    File histFile = SPIFFS.open(fileName, "w");
    if (!histFile)
    {
        ILogger::log(l_error, "Failed to open history file for writing %s", fileName);
        return true;
    }

    int count = 0;
    for (int i = 0; i < usedEntries; i++)
    {
        if (lines[i].logTime > historyLimit)
        {
            count++;
            histFile.print(lines[i].logTime);
            histFile.print(",");
            histFile.println(lines[i].message);
        }
    }

    ILogger::log(l_info, "Saved log history file, entries = %d", count);
    // Close the file
    histFile.close();
    unSavedLines = false;
    return true;
}

bool FileLog::full()
{
    return usedEntries >= numberOfLines;
}

void FileLog::shuffledown()
{
    for (int i = 0; i < numberOfLines - 1; i++)
    {
        lines[i].logTime = lines[i + 1].logTime;
        strncpy(lines[i].message, lines[i + 1].message, MAX_LINE_LENGTH);
        usedEntries--;
    }
}

void FileLog::clear()
{
    usedEntries = 0;
    saveToDisk();
}

void FileLog::appendWebPageData(StringBuilder & stringBuilder)
{

    stringBuilder.append("[ {  \"syslog\" : \"")
        .append("<table>");

    if (isEmpty())
    {
        stringBuilder.append("<tr><td>%S</td></tr>", Language::phrase(INT_PHRASE_NO_DATA));
    }
    else
    {
        for (int i = 0; i < usedEntries; i++)
        {
            time_t local = lines[i].logTime;

            struct tm *timeinfo = localtime(&local);

            stringBuilder.append("<tr><td>%d/%d %d:%.2d:%.2d</td><td>%s</td></tr>",
                                 timeinfo->tm_mon + 1,
                                 timeinfo->tm_mday,
                                 timeinfo->tm_hour,
                                 timeinfo->tm_min,
                                 timeinfo->tm_sec,
                                 lines[i].message);
        }
    }

    stringBuilder.append("</table>")
        .append("\"}]");

}