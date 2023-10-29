//!
//! @file MQTTClient.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the MQTTClient interface
//!
//! @copyright Copyright (c) 2023
//!
#include "MQTTClient.hpp"
#include "WiFiHandler.hpp"
#include "Logger.hpp"

namespace ModelController
{
    //!
    //! @brief Read message and set value to input variable
    //!
    void MQTTClient::callback(char* topic, byte* message, unsigned int length)
    {
        Logger::debug("MQTT callback called");
        std::string topicName(topic);
        Logger::debug("Topic: " + topicName);
        if (inputVariables.count(topic))
        {
            IMQTTInput* inputVar = inputVariables[topic];

            std::string messageTemp;
            for (int i = 0; i < length; i++)
            {
                messageTemp += (char)message[i];
            }
            Logger::debug("Message: " + messageTemp);

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
            Logger::info("MQTT connected");
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
    BaseModule* MQTTClient::GetChild(std::string modulePath, ModuleType type, ModuleDataType dataType)
    {
        Logger::trace("MQTTClient::GetChild(" + modulePath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        BaseModule* child = nullptr;
        child = BaseModule::GetChild(modulePath, type, dataType);
        if (child == nullptr)
        {
            Logger::trace("No MQTT input/ouput matches parameters");
            if (type == ModuleType::eInput)
            {
                Logger::trace("Creating new MQTT output variable with type " + DataTypeToString(dataType));
                IMQTTOutput* outputVar = nullptr;
                std::function<bool(std::string, std::string)> pubFn = [&](std::string topic, std::string value) -> bool{ return this->publish(topic, value);};
                switch (dataType)
                {
                    case ModuleDataType::eUndefined:
                        break;
                    case ModuleDataType::eNone:
                        break;
                    case ModuleDataType::eDouble:
                        {
                            child = new MQTTOutput<double>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eFloat:
                        {
                            child = new MQTTOutput<float>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eString:
                        {
                            child = new MQTTOutput<std::string>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt8:
                        {
                            child = new MQTTOutput<int8_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt16:
                        {
                            child = new MQTTOutput<int16_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt32:
                        {
                            child = new MQTTOutput<int32_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt64:
                        {
                            child = new MQTTOutput<int64_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt8:
                        {
                            child = new MQTTOutput<uint8_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt16:
                        {
                            child = new MQTTOutput<uint16_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt32:
                        {
                            child = new MQTTOutput<uint32_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt64:
                        {
                            child = new MQTTOutput<uint64_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eBool:
                        {
                            child = new MQTTOutput<bool>(modulePath, pubFn, this);
                        }
                        break;
                    default:
                        break;
                }
            }
            else if (type == ModuleType::eOutput)
            {
                Logger::trace("Creating new MQTT input variable with type " + DataTypeToString(dataType));
                IMQTTInput* inputVar = nullptr;
                switch (dataType)
                {
                    case ModuleDataType::eUndefined:
                        break;
                    case ModuleDataType::eNone:
                        break;
                    case ModuleDataType::eDouble:
                        {
                            MQTTInput<double>* input = new MQTTInput<double>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eFloat:
                        {
                            MQTTInput<float>* input = new MQTTInput<float>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eString:
                        {
                            MQTTInput<std::string>* input = new MQTTInput<std::string>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt8:
                        {
                            MQTTInput<int8_t>* input = new MQTTInput<int8_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt16:
                        {
                            MQTTInput<int16_t>* input = new MQTTInput<int16_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt32:
                        {
                            MQTTInput<int32_t>* input = new MQTTInput<int32_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt64:
                        {
                            MQTTInput<int64_t>* input = new MQTTInput<int64_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt8:
                        {
                            MQTTInput<uint8_t>* input = new MQTTInput<uint8_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt16:
                        {
                            MQTTInput<uint16_t>* input = new MQTTInput<uint16_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt32:
                        {
                            MQTTInput<uint32_t>* input = new MQTTInput<uint32_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt64:
                        {
                            MQTTInput<uint64_t>* input = new MQTTInput<uint64_t>(modulePath, this);
                            inputVar = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eBool:
                        {
                            MQTTInput<bool>* input = new MQTTInput<bool>(modulePath, this);
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
                    Logger::debug("Add MQTT input variable /" + inputVar->GetName() + " to inputVariables and subscribe");
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
        : BaseConnector(name, config, parent, ModuleType::eNone, ModuleDataType::eNone),
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
