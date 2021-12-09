// Setting specific to the Olimex POE Ethernet board
#ifdef ARDUINO_ESP32_POE_ISO
    #define USE_ETHERNET

    #define USE_DISPLAY
    #define PIN_SCL 16
    #define PIN_SDA 13


#endif 
//ARDUINO_ESP32_POE_ISO


#ifdef ARDUINO_ESP32_GATEWAY
    #define USE_ETHERNET
#endif //ARDUINO_ESP32_GATEWAY


#ifdef ARDUINO_LOLIN_D32 
    // ---------- SETTINGS FOR LOLIN 32 BOARD ----------
    #define USE_WIFI
    #define HOSTNAME    "LEDs"

    // oled
    #define USE_DISPLAY
    #define DISPLAY_OLED_128X64
    #define PIN_SCL 4
    #define PIN_SDA 5
    #define DISPLAY_ON_PIN 14

    // for the leds
    #define LED_DATA_PIN 16
    
    // enable safe mode when grounded
    #define SAFE_MODE_PIN 25

    
#endif //ARDUINO_LOLIN_D32


#ifdef ARDUINO_ESP32_DEV
    // ---------- SETTINGS FOR DEVKIT V1 AND DEVKIT V4 BOARDS ----------
    #define USE_WIFI
    #define HOSTNAME    "Lights"

    // for the leds
    #define LED_DATA_PIN 4

    // enable safe mode when grounded
    #define SAFE_MODE_PIN 25

    #define WIFI_HOTSPOT_ACTIVE_PIN  18

#endif //ARDUINO_ESP32_DEV


#ifdef ARDUINO_LOLIN_D32_PRO
    #define USE_WIFI
#endif    