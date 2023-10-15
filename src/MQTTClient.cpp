//!
//! @file MQTTClient.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the MQTTClient interface
//!
//! @copyright Copyright (c) 2023
//!
#include "MQTTClient.hpp"
#include "WiFiHandler.hpp"

namespace ModelController
{
    //!
    //! @brief Read message and set value to input variable
    //!
    void MQTTClient::callback(char* topic, byte* message, unsigned int length)
    {
        std::string topicName(topic);

        if (inputVariables.count(topic))
        {
            IMQTTInput* inputVar = inputVariables[topic];

            std::string messageTemp;
            for (int i = 0; i < length; i++)
            {
                messageTemp += (char)message[i];
            }

            inputVar->SetMQTTValue(messageTemp);
        }

    }
    //!
    //! @brief Generate/Read client ID and reconnect to MQTT broker
    //!
    //! Subscribe to input topics
    //!
    void MQTTClient::reconnect()
    {
        std::string clientID = this->clientID;
        if (clientID.empty())
        {
            clientID += "ESP32";
            // ToDo: Generate random ID (mqtt client id size): clientID += esp_random();
        }
        if (client.connect(clientID.c_str()))
        {
            Serial.println("MQTT connected");
            for (std::pair<std::string, IMQTTInput*> kvp : inputVariables)
            {
                subscribe(("/" + kvp.second->GetName()).c_str());
            }
        }
    }
    //!
    //! @brief Call subscribe from PubSubClient
    //!
    bool MQTTClient::subscribe(std::string topic)
    {
        return client.subscribe(topic.c_str());
    }
    //!
    //! @brief Get input/ouput mqtt variable, generate new if none found with given properties
    //!
    BaseConnector* MQTTClient::GetChild(std::string connectorPath, ConnectorType type, ConnectorDataType dataType)
    {
        BaseConnector* child = nullptr;
        child = BaseConnector::GetChild(connectorPath, type, dataType);
        if (child == nullptr)
        {
            if (type == ConnectorType::eOutput)
            {
                Serial.println("Output");
                IMQTTOutput* outputVar = nullptr;
                std::function<bool(std::string, std::string)> pubFn = [&](std::string topic, std::string value) -> bool{ return this->publish(topic, value);};
                switch (dataType)
                {
                    case ConnectorDataType::eUndefined:
                        break;
                    case ConnectorDataType::eNone:
                        break;
                    case ConnectorDataType::eDouble:
                        {
                            child = new MQTTOutput<double>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eFloat:
                        {
                            child = new MQTTOutput<float>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eString:
                        {
                            child = new MQTTOutput<std::string>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eInt8:
                        {
                            child = new MQTTOutput<int8_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eInt16:
                        {
                            child = new MQTTOutput<int16_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eInt32:
                        {
                            child = new MQTTOutput<int32_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eInt64:
                        {
                            child = new MQTTOutput<int64_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eUInt8:
                        {
                            child = new MQTTOutput<uint8_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eUInt16:
                        {
                            child = new MQTTOutput<uint16_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eUInt32:
                        {
                            child = new MQTTOutput<uint32_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eUInt64:
                        {
                            child = new MQTTOutput<uint64_t>(connectorPath, pubFn, this);
                        }
                        break;
                    case ConnectorDataType::eBool:
                        {
                            child = new MQTTOutput<bool>(connectorPath, pubFn, this);
                        }
                        break;
                    default:
                        break;
                }
            }
            else if (type == ConnectorType::eInput)
            {
                IMQTTInput* inputVar = nullptr;
                switch (dataType)
                {
                    case ConnectorDataType::eUndefined:
                        break;
                    case ConnectorDataType::eNone:
                        break;
                    case ConnectorDataType::eDouble:
                        {
                            MQTTInput<double>* input = new MQTTInput<double>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eFloat:
                        {
                            MQTTInput<float>* input = new MQTTInput<float>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eString:
                        {
                            MQTTInput<std::string>* input = new MQTTInput<std::string>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eInt8:
                        {
                            MQTTInput<int8_t>* input = new MQTTInput<int8_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eInt16:
                        {
                            MQTTInput<int16_t>* input = new MQTTInput<int16_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eInt32:
                        {
                            MQTTInput<int32_t>* input = new MQTTInput<int32_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eInt64:
                        {
                            MQTTInput<int64_t>* input = new MQTTInput<int64_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eUInt8:
                        {
                            MQTTInput<uint8_t>* input = new MQTTInput<uint8_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eUInt16:
                        {
                            MQTTInput<uint16_t>* input = new MQTTInput<uint16_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eUInt32:
                        {
                            MQTTInput<uint32_t>* input = new MQTTInput<uint32_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eUInt64:
                        {
                            MQTTInput<uint64_t>* input = new MQTTInput<uint64_t>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ConnectorDataType::eBool:
                        {
                            MQTTInput<bool>* input = new MQTTInput<bool>(connectorPath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    default:
                        break;
                }
                if (inputVar != nullptr)
                {
                    // ToDo: Delete from input variables, if destructor called (idea: via events)
                    inputVariables["/" + inputVar->GetName()] = inputVar;
                    subscribe("/" + inputVar->GetName());
                }
            }
        }
        return child;
    }
    //!
    //! @brief Construct a new MQTTClient::MQTTClient object
    //!
    MQTTClient::MQTTClient(std::string name, JsonObject config, BaseConnector* parent)
        : BaseConnector(name, config, parent, ConnectorType::eNone, ConnectorDataType::eNone),
        client(wifiClient),
        OnSTAConnected(&WiFiHandler::STAConnected, [&](){reconnect();}),
        loopListener([&](){ Serial.println("MQTT loop"); this->client.loop(); }, 0)
    {
        JsonVariant jPort = config["port"];
        serverPort = DefaultPort;
        if (jPort.is<uint16_t>())
        {
            serverPort = jPort.as<uint16_t>();
        }
        JsonVariant jServer = config["server"];
        serverHostname = DefaultServer;
        if (jServer.is<std::string>())
        {
            serverHostname = jServer.as<std::string>();
        }
        JsonVariant jClientID = config["clientID"];
        if (jClientID.is<std::string>())
        {
            clientID = jClientID.as<std::string>();
        }
        client.setServer(serverHostname.c_str(), serverPort);
        client.setCallback([&](char* topic, byte* message, unsigned int length){this->callback(topic, message, length);});
    }
    //!
    //! @brief Calls publish from PubSubClient
    //!
    bool MQTTClient::publish(std::string topic, std::string value)
    {
        return client.publish(topic.c_str(), value.c_str());
    }
} // namespace ModelController
