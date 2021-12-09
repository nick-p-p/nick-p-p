#pragma once
#include <ILogger.h>

#define NTP_SERVER "pool.ntp.org"


#include <MyServer.h>

#define SYSTEM_ID_LEN 12

/**
 * @brief class for abstracting the details of network as Wifi or Ethernet
 * 
 * provides a web server
 */ 
class MyNetwork
{
public:
    MyNetwork();

    /**
     * @brief starts up the network
     * 
     * @param safeMode - if true the webserver is started with minimal (only file upload) capabilities
     * @param hostname - network name for DNS
     */ 
    virtual void begin(bool safeMode, String hostname, int port = 80);


    virtual void loop();

    /**
	 * @brief returns the MAC address of this device with no colons
	 **/
    static char *MacId();

    /**
     * @brief the type of message being sent
     */ 
    enum NetMessage { 
        /**
         * @brief state of the network connection
         */ 
        state, 

        /**
         * @brief allocated local IP address
         */ 
        IP, 

        /**
         * @brief SSID of concected wifi network
         */ 
        SSID, 
        
        /**
         * @brief ?
         */ 
        WifiMode};

    /**
     * @brief sets a callbck for status messages e.g. to update a display
     */ 
    void setMessageCallback( void (*func)(NetMessage, const char*) );

    

protected:
    /**
	 * should be called as soon as a mac address is available.
	 * the mac address is used to generate a unique identifier for this system
	 */
    static void setMac(const char *mac);

    /**
     * @brief connect and wait until connection confirmed
     * implementation specific
     */ 
    virtual void waitForConnection() = 0;

    /**
     * @brief the webserver
     */ 
    static MyServer server;

    static char macId[];

    /**
     * @brief notify any callback with status updates
     */ 
    void notify (NetMessage netMessage, const char * msg);

    /**
     * @brief the registered callback if any
     */ 
    void (*_messsgeCallback)(NetMessage netMessage, const char* msg) = NULL;
};
