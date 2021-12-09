#pragma once
#include <Arduino.h>

class StringBuilder
{

    /* data */
public:
    StringBuilder(int capacity);
	~StringBuilder();
	// StringBuilder& append(const char * data);
	StringBuilder& append(int iData);
	StringBuilder& append(const __FlashStringHelper* value);
	StringBuilder& append(String text);
	StringBuilder& append(const char *msg, ...);
	StringBuilder& append(char c);
    char * data();
	int length();
	StringBuilder&clear();
    StringBuilder& removeBreaks();

private:
	char * buffer = nullptr;
	int currentPosition = 0;
	int capacity = 0;

};

