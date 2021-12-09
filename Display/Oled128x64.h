#pragma once
// #include "core/config/BoardsConfig.h"


#include <Display.h>
#include <ILogger.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#include <Wire.h>
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>

class Oled128x64 : public Display
{
public:
    Oled128x64();
    virtual void begin(int sda, int scl);
    
    virtual void setActive(bool active);

protected:
    virtual void update(int reason, const char *msg);

private:
    void clearAndSetText();

    void draw();

    int printAndMoveIfSet(int y, int offset, const char * prefixe, String value);
    bool displayOn = true;

    Adafruit_SSD1306 oled = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    String netStatus;
    String IPaddress;
    String SSID;
    String wifiMode;
    String palleteName;
    String mode;

    bool displayActive = true;
};

