#include "Oled128x64.h"



Oled128x64::Oled128x64() : Display(this)
{
}

void Oled128x64::begin(int sda, int scl)
{
    ILogger::log(l_info, "Starting Oled128x64");
    ILogger::log(l_info, "Wire: SDA=%d, SCL=%d", sda, scl);
    Wire.begin(PIN_SDA, PIN_SCL);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    {
        ILogger::log(l_error,"SSD1306 allocation failed");
        for (;;)
            ; // Don't proceed, loop forever
    }
    //oled.setFont(&FreeSerif9pt7b);
    oled.display();
    oled.clearDisplay();
}

void Oled128x64::setActive(bool active)
{
    if (displayActive != active)
    {
        displayActive = active;
        ILogger::log(l_info, "Display set %s", displayActive ? "on" : "off");
        draw();
    }
}

void Oled128x64::update(int reason, const char *msg)
{
    if (reason == (int)DisplayReason::networkStatus)
    {
        netStatus = msg;
    }
    else if (reason == (int)DisplayReason::IPaddress)
    {
        IPaddress = msg;
    }
    else if (reason == (int)DisplayReason::SSID)
    {
        SSID = msg;
    }
    else if (reason == (int)DisplayReason::WifiMode)
    {
        wifiMode = msg;
    }
    else if (reason == (int)DisplayReason::paletteName)
    {
        palleteName = msg;
    }
        else if (reason == (int)DisplayReason::paletteNameFixed)
    {
        palleteName = "*" + String(msg);
    }
    else if (reason == (int)DisplayReason::safeMode)
    {
        mode = msg;
    }

    draw();
}

void Oled128x64::clearAndSetText()
{
    oled.clearDisplay();
    oled.setTextSize(1);      // Normal 1:1 pixel scale
    oled.setTextColor(WHITE); // Draw white text

    oled.setCursor(0, 0);
}

void Oled128x64::draw()
{
    clearAndSetText();
    if (displayActive)
    {
        int verticalSpacing = (mode.length() == 0) ? 12 : 10;
        int y = 0;
        y = printAndMoveIfSet(y, verticalSpacing, "", mode);
        y = printAndMoveIfSet(y, verticalSpacing, "", netStatus);
        y = printAndMoveIfSet(y, verticalSpacing, "IP: ", IPaddress);
        y = printAndMoveIfSet(y, verticalSpacing, "SSID: ", SSID);
        y = printAndMoveIfSet(y, verticalSpacing, "Mode: ", wifiMode == 0 ? "Home network" : "Soft AP");
        y = printAndMoveIfSet(y, verticalSpacing, "Now: ", palleteName);
    }
    oled.display();
}

int Oled128x64::printAndMoveIfSet(int y, int offset, const char *prefix, String value)
{
    if (value.length() == 0)
    {
        return y;
    }
    oled.setCursor(0, y);
    oled.print(prefix);
    oled.print(value);
    return y + offset;
}

