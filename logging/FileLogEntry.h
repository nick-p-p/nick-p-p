#define MAX_LINE_LENGTH 80
#include <Arduino.h>



struct FileLogEntry 
{
  public:
    time_t logTime;
    char  message [MAX_LINE_LENGTH+1] ;
};