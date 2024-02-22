//!
//! @file MQTTClient.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the MQTTClient interface
//!
//! @copyright Copyright (c) 2023
//!
#include "MQTTClient.hpp"
#include "ModuleIn.hpp"
#include "WiFiHandler.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

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
        if (mqttInputVariables.count(topic))
        {
            IModuleOut* outputVar = mqttInputVariables[topic];

            std::string messageTemp;
            for (int i = 0; i < length; i++)
            {
                messageTemp += (char)message[i];
            }
            Logger::debug("Message: " + messageTemp);

            outputVar->SetStringValue(messageTemp);
        }

    }
    //!
    //! @brief Generate/Read client ID and reconnect to MQTT broker
    //!
    //! Subscribe to input topics
    //!
    void MQTTClient::reconnect()
    {
        // Conversion of serverHostname to string and in second step to const char* is needed, because otherwise the domain in client is overridden by the client ID (seems to be a bug in PubSubClient)
        std::string hostnameStr = serverHostname.GetValue();
        const char* hostname = hostnameStr.c_str();
        client.setServer(hostname, serverPort.GetValue());
        Logger::debug(std::string("Connecting to MQTT server on ") + hostname + ":" + std::to_string(serverPort));
        if (client.connect(clientID.GetValue().c_str()))
        {
            Logger::info("MQTT connected");
            for (std::pair<std::string, IModuleOut*> kvp : mqttInputVariables)
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
        topic = "/" + edgeName + topic;
        Logger::debug("Subscribing to " + topic);
        return client.subscribe(topic.c_str());
    }
    //!
    //! @brief Get input/ouput mqtt variable, generate new if none found with given properties
    //!
    BaseModule* MQTTClient::GetChild(std::string modulePath, ModuleType type, ModuleDataType dataType, bool recursive)
    {
        Logger::trace("MQTTClient::GetChild(" + modulePath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        BaseModule* child = nullptr;
        child = BaseModule::GetChild(modulePath, type, dataType, recursive);
        if (child == nullptr)
        {
            Logger::trace("No MQTT input/ouput matches parameters");
            if (type == ModuleType::eInput)
            {
                Logger::trace("Creating new MQTT output variable with type " + DataTypeToString(dataType));
                IModuleIn* outputVar = nullptr;
                modulePath = Utils::TrimStart(modulePath, "/");
                std::string topic = "/" + modulePath;
                std::function<void(std::string)> pubFn = [&, topic](std::string value) { this->publish(topic, value);};
                switch (dataType)
                {
                    case ModuleDataType::eUndefined:
                        break;
                    case ModuleDataType::eNone:
                        break;
                    case ModuleDataType::eDouble:
                        {
                            child = new ModuleIn<double>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eFloat:
                        {
                            child = new ModuleIn<float>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eString:
                        {
                            child = new ModuleIn<std::string>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt8:
                        {
                            child = new ModuleIn<int8_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt16:
                        {
                            child = new ModuleIn<int16_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt32:
                        {
                            child = new ModuleIn<int32_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eInt64:
                        {
                            child = new ModuleIn<int64_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt8:
                        {
                            child = new ModuleIn<uint8_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt16:
                        {
                            child = new ModuleIn<uint16_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt32:
                        {
                            child = new ModuleIn<uint32_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eUInt64:
                        {
                            child = new ModuleIn<uint64_t>(modulePath, pubFn, this);
                        }
                        break;
                    case ModuleDataType::eBool:
                        {
                            child = new ModuleIn<bool>(modulePath, pubFn, this);
                        }
                        break;
                    default:
                        break;
                }
            }
            else if (type == ModuleType::eOutput)
            {
                Logger::trace("Creating new MQTT input variable with type " + DataTypeToString(dataType));
                IModuleOut* mqttInput = nullptr;
                switch (dataType)
                {
                    case ModuleDataType::eUndefined:
                        break;
                    case ModuleDataType::eNone:
                        break;
                    case ModuleDataType::eDouble:
                        {
                            ModuleOut<double>* input = new ModuleOut<double>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eFloat:
                        {
                            ModuleOut<float>* input = new ModuleOut<float>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eString:
                        {
                            ModuleOut<std::string>* input = new ModuleOut<std::string>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt8:
                        {
                            ModuleOut<int8_t>* input = new ModuleOut<int8_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt16:
                        {
                            ModuleOut<int16_t>* input = new ModuleOut<int16_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt32:
                        {
                            ModuleOut<int32_t>* input = new ModuleOut<int32_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eInt64:
                        {
                            ModuleOut<int64_t>* input = new ModuleOut<int64_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt8:
                        {
                            ModuleOut<uint8_t>* input = new ModuleOut<uint8_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt16:
                        {
                            ModuleOut<uint16_t>* input = new ModuleOut<uint16_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt32:
                        {
                            ModuleOut<uint32_t>* input = new ModuleOut<uint32_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eUInt64:
                        {
                            ModuleOut<uint64_t>* input = new ModuleOut<uint64_t>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    case ModuleDataType::eBool:
                        {
                            ModuleOut<bool>* input = new ModuleOut<bool>(modulePath, this);
                            mqttInput = input;
                            child = input;
                        }
                        break;
                    default:
                        break;
                }
                if (mqttInput != nullptr)
                {
                    // ToDo: Delete from input variables, if destructor called (idea: via events)
                    Logger::debug("Add MQTT input variable /" + mqttInput->GetName() + " to inputVariables and subscribe");
                    mqttInputVariables["/" + edgeName + "/" + mqttInput->GetName()] = mqttInput;
                    subscribe("/" + mqttInput->GetName());
                }
            }
        }
        return child;
    }
    //!
    //! @brief Construct a new MQTTClient::MQTTClient object
    //!
    MQTTClient::MQTTClient(std::string name, JsonObject config, BaseModule* parent)
        : BaseContainer(name, config, parent, ModuleType::eNone, ModuleDataType::eNone),
        client(wifiClient),
        OnSTAConnected(&WiFiHandler::STAConnected, [&](){reconnect();}),
        loopListener([&](){ Logger::trace("MQTT loop"); this->client.loop(); }, 0),
        serverHostname("server", config, "raspberrypi", this),
        serverPort("port", config, 1883, this),
        clientID("clientID", config, "ESP32-" + Utils::GetRandomNumber(18), this)
    {
        client.setCallback([&](char* topic, byte* message, unsigned int length){this->callback(topic, message, length);});

        IModuleOut::ModuleOutCreated(this->GetPath() + "/*");
        IModuleIn::ModuleInCreated(this->GetPath() + "/*");
    }
    //!
    //! @brief Calls publish from PubSubClient
    //!
    bool MQTTClient::publish(std::string topic, std::string value)
    {
        Logger::trace("MQTT " + GetPath() + " publish " + value + " to " + topic);
        return client.publish(("/" + edgeName + topic).c_str(), value.c_str(), true);
    }
} // namespace ModelController
