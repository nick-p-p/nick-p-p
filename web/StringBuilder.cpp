#include "StringBuilder.h"

StringBuilder::StringBuilder(int capacity)
{
    this->capacity = capacity;
	buffer = new char[capacity + 1];
	buffer[capacity] = '\0';
	clear();
}

StringBuilder::~StringBuilder()
{
    delete[] buffer;
}



// StringBuilder& StringBuilder::append(const char *data)
// {
// 	int dataLength = strlen(data);
// 	if (currentPosition + dataLength < capacity)
// 	{
// 		strcpy(&buffer[currentPosition], data);
// 		currentPosition += dataLength;
// 		buffer[currentPosition] = 0;
// 	}
// 	else
// 	{
// 		Serial.printf("Error appending string, not enough space for: %s\r\n", data);
// 	}
// 	return *this;
// }

StringBuilder& StringBuilder::append(String text)
{
	return append(text.c_str());
}

StringBuilder& StringBuilder::append(int iData)
{
	char tbuffer[20];
	sprintf(tbuffer, "%d", iData);
	return append(tbuffer);
}

StringBuilder& StringBuilder::append(const __FlashStringHelper *value)
{
	size_t dataLength = value == NULL
							? 0
							: strlen_P(reinterpret_cast<const char *>(value));

	if (dataLength && (currentPosition + dataLength < capacity))
	{
		strncpy_P(&buffer[currentPosition], reinterpret_cast<const char *>(value), dataLength);
		currentPosition += dataLength;
		buffer[currentPosition] = 0;
	}
	return *this;
}

StringBuilder& StringBuilder::append(const char *msg, ...)
{
    //int vsnprintf( char *restrict buffer, size_t bufsz, const char *restrict format, va_list vlist );

	if ((capacity - currentPosition) < strlen(msg))
	{
		return *this;
	}

	va_list argList;
	va_start(argList, msg);
	int used = vsnprintf(&buffer[currentPosition], capacity-currentPosition, msg, argList);
	va_end(argList);
	currentPosition += used; //strlen(buffer);

	return *this;
}


StringBuilder& StringBuilder::append(char c)
{
	if (currentPosition < capacity)
	{
		buffer[currentPosition] = c;
		currentPosition++;
		buffer[currentPosition] = 0;
	}
	return *this;
}

char *StringBuilder::data()
{
	return buffer;
}

int StringBuilder::length()
{
	return currentPosition;
}


StringBuilder& StringBuilder::clear()
{
	buffer[0] = 0;
	currentPosition = 0;
	return *this;
}

StringBuilder& StringBuilder::removeBreaks()
{
	int offset = 0;
	for (int i = 0; i < capacity; i++)
	{
		switch (buffer[i])
		{
		case '\r':
		case '\n':
		case '\t':
			offset++;
			currentPosition--;
			break;

		default:
			buffer[i - offset] = buffer[i];
			break;
		}

		if (buffer[i] == '\0')
		{
			return *this;
		}
	}
	return *this;
}