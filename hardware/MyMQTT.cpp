#include "MyMQTT.h"

#define TOPICS_REQUIRED_MSG "If a server is specified, input and output topics cannot be blank"
#define SERVER_REQUIRED_MSG "Server is required for active modes"

MyMQTT *MyMQTT::instance = nullptr;


MyMQTT::MyMQTT(PubSubClient *mqttClient)
{
    this->mqttClient = mqttClient;
    instance = this;
}

void MyMQTT::begin()
{

    hasConfig = false; //load();
    if (!hasConfig)
    {
        sprintf(inputTopic, DEFAULT_MQTT_INPUT_TOPIC);//in", MyNetwork::MacId());
        sprintf(outputTopic, DEFAULT_MQTT_OUTPUT_TOPIC);//, MyNetwork::MacId());
        sprintf(mqttServer, DEFAULT_MQTT_SERVER);
        save();
    }

    sprintf(clientId, "CL%s", MyNetwork::MacId());
    myFromAddress = MyNetwork::MacId();
    mqttBegin();

    // MyServer::subscribe(this);
}


void MyMQTT::loop()
{
    if (!mqttClient->connected())
    {
        reconnect();
    }

    if (mqttClient->connected())
    {
        mqttClient->loop();
        if (!mqttClient->connected())
        {
            ILogger::log(l_error, "Whoa mqtt disconnected after loop");
        }
    }
}

void MyMQTT::handleInbound(char *topic, byte *message, unsigned int length){}
void MyMQTT::onSave(JsonDocument & doc){}
void MyMQTT::onLoad(JsonDocument & doc){}

void MyMQTT::reconnect()
{
    // Loop until we're reconnected
    if (!mqttClient->connected())
    {
        ILogger::log(l_info, "Attempting MQTT connection for client %s", clientId);
        // Attempt to connect
        if (mqttClient->connect(clientId))
        {
            if (!mqttClient->connected())
            {
                ILogger::log(l_error, "Whoa mqtt not connected after succesful connext!");
                delay(5000);
                return;
            }

            ILogger::log(l_info, "connected");
            // Subscribe

            mqttClient->subscribe(inputTopic);

            if (!mqttClient->connected())
            {
                ILogger::log(l_error, "Whoa mqtt disconnected after subscribe to %s", inputTopic);
                delay(5000);
                return;
            }
        }
        else
        {
            ILogger::log(l_error, "failed, rc = %d try again in 5 seconds", mqttClient->state());
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MyMQTT::callback(char *topic, byte *message, unsigned int length)
{
    instance->handleCallback(topic, message, length);
}


void MyMQTT::handleCallback(char *topic, byte *message, unsigned int length)
{
    handleInbound(topic, message, length);
}



void MyMQTT::mqttBegin()
{
    ILogger::log(l_info, "Setting MQTT server to: %s:1883", mqttServer);
    mqttClient->setServer(mqttServer, 1883);
    mqttClient->setCallback(MyMQTT::callback);
}

bool MyMQTT::load()
{
    ILogger::log(l_info, "loading: %s", MQTT_FILENAME);

    File mqttFile = SPIFFS.open(MQTT_FILENAME, "r");
    if (!mqttFile)
    {
        ILogger::log(l_error, "Failed open to open MQTT file: %s", MQTT_FILENAME);
        return false;
    }

    StaticJsonDocument<256> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, mqttFile);
    if (error)
    {
        ILogger::log(l_error, "Failed to read file, using default configuration: %s", MQTT_FILENAME);
        return false;
    }

    // mode = (MQTTMode)(doc["mode"].as<byte>());
    sprintf(mqttServer, "%s", doc["server"].as<String>().c_str());
    sprintf(inputTopic, "%s", doc["inputTopic"].as<String>().c_str());
    sprintf(outputTopic, "%s", doc["outputTopic"].as<String>().c_str());

    // ILogger::log(l_info, "MQTT mode: %s", modeText());
    ILogger::log(l_info, "server: %s", mqttServer);
    ILogger::log(l_info, "inputTopic: %s", inputTopic);
    ILogger::log(l_info, "outputTopic: %s", outputTopic);

    onLoad(doc);
    mqttFile.close();

    return (strlen(mqttServer) > 0) && (strlen(inputTopic) > 0) && (strlen(outputTopic) > 0);
}

void MyMQTT::save()
{
    // Delete existing file, otherwise the configuration is appended to the file
    SPIFFS.remove(MQTT_FILENAME);

    File mqttFile = SPIFFS.open(MQTT_FILENAME, "w");
    if (!mqttFile)
    {
        ILogger::log(l_error, "Failed to open mqtt config file for writing: %s", MQTT_FILENAME);
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<256> doc;

    // Set the values in the document
    // doc["mode"] = mode;
    doc["server"] = mqttServer;
    doc["inputTopic"] = inputTopic;
    doc["outputTopic"] = outputTopic;

    onSave(doc);
    // Serialize JSON to file
    if (serializeJson(doc, mqttFile) == 0)
    {
        ILogger::log(l_error, "Failed to write to mqtt config file");
    }

    mqttFile.close();
}