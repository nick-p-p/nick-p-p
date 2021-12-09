#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <MyNetwork.h>
#include <ArduinoJson.h>

#ifndef DEFAULT_MQTT_INPUT_TOPIC
#define DEFAULT_MQTT_INPUT_TOPIC "/input/data"
#endif

#ifndef DEFAULT_MQTT_OUTPUT_TOPIC
#define DEFAULT_MQTT_OUTPUT_TOPIC "/output/data"
#endif

#ifndef DEFAULT_MQTT_SERVER
#define DEFAULT_MQTT_SERVER ""
#endif

// file for config
#define MQTT_FILENAME "/mqttconfig.json"

#define SERVER_NAME_LEN 20
#define TOPIC_NAME_LEN 60


class MyMQTT
{

public:
    MyMQTT(PubSubClient *mqttClient);

    virtual void begin();

    void loop();

protected:

    /**
     * override to handle any incoming messages
     */ 
    virtual void handleInbound(char *topic, byte *message, unsigned int length);

    /**
     * @brief   overload to add any data to doc before the data is serialized to file
     */ 
    virtual void onSave(JsonDocument & doc);

    /**
     * @brief   overload to add any data to doc before the data is serialized to file
     */ 
    virtual void onLoad(JsonDocument & doc);



    /**
     * try and connect if disconnected
     */
    void reconnect();

    bool hasConfig = false;

     /** 
     * the client we use
     */
    PubSubClient *mqttClient;

    String myFromAddress = "";

    char mqttServer[SERVER_NAME_LEN + 1] = "";
    char inputTopic[TOPIC_NAME_LEN + 1] = "";
    char outputTopic[TOPIC_NAME_LEN + 1] = "";
    char clientId[30];
private:

     /**
     * pointer to singleton instance
     */
    static MyMQTT *instance;

    /**
     * callback for when a message is received on the inbound topic
     */
    static void callback(char *topic, byte *message, unsigned int length);
    void handleCallback(char *topic, byte *message, unsigned int length);
    
    /**
     * configure the mqtt server, topics and callback
     */ 
    void mqttBegin();

    /**
     * load configuration data
     */ 
    bool load();
    /**
     * save configuration data
     */ 
    void save();


    


};

