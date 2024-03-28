//!
//! @file MQTTClient.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Wrapper for MQTTClient
//!
//! @copyright Copyright (c) 2023
//!
#pragma once

#include "PubSubClient.h"
#include "BaseContainer.hpp"
#include "WiFi.h"
#include <map>
#include "IModuleOut.hpp"
#include "LoopEvent.hpp"
#include "ConfigItem.hpp"

namespace ModelController
{
    class MQTTClient : public BaseContainer
    {
        private:
            //!
            //! @brief Listener for periodic task
            //!
            LoopEvent::LoopListener loopListener;
            //!
            //! @brief List of input variables (output of the mqtt client)
            //!
            std::map<std::string, IModuleOut*> mqttInputVariables;
            //!
            //! @brief Hostname of the MQTT broker
            //!
            ConfigItem<std::string> serverHostname;
            //!
            //! @brief MQTT brokers port
            //!
            ConfigItem<uint16_t> serverPort;
            //!
            //! @brief ClientID (name of the client)
            //!
            ConfigItem<std::string> clientID;
            //!
            //! @brief PubSubClient to handle MQTT messages
            //!
            PubSubClient client;
            //!
            //! @brief WiFiClient needed for PubSubClient
            //!
            WiFiClient wifiClient;
            //!
            //! @brief Callback on sent subscribed topic
            //!
            //! @param topic Topic, to which message was sent
            //! @param message Message sent to topic
            //! @param length Length of the message
            //!
            void callback(char* topic, byte* message, unsigned int length);
            //!
            //! @brief Listener called, if WiFi-STA connected
            //!
            Event<>::Listener OnSTAConnected;
            //!
            //! @brief Reconnect to MQTT broker
            //!
            void reconnect();
            //!
            //! @brief Subscribe to topic
            //!
            //! @param topic Topic to subscribe to
            //! @return true Success
            //! @return false Failure
            //!
            bool subscribe(std::string topic);
        protected:
            //!
            //! @brief Get child of the object
            //!
            //! @param connectorPath Path of the child
            //! @param type ModuleType of the child
            //! @param dataType ModuleDataType of the child
            //! @param recursive If set to false, child is returned, with which's name the modulePath starts
            //! @return BaseModule* Child, nullptr if not found
            //!
            virtual BaseModule* GetChild(std::string connectorPath, ModuleType type = ModuleType::eUndefined, ModuleDataType dataType = ModuleDataType::eUndefined, bool recursive = false) override;
        public:
            //!
            //! @brief Name of the type for config
            //!
            static constexpr const char* type = "mqtt";
            //!
            //! @brief Construct a new MQTTClient object
            //!
            //! @param name Name of the MQTTClient connection
            //! @param config Config of the connection
            //! @param parent Parent of the MQTTClient-Connection (normally pass this)
            //!
            MQTTClient(std::string name, JsonObject config, BaseModule* parent = nullptr);
            //!
            //! @brief Get the type of the container
            //!
            //! @return std::string Type of the container in string representation
            //!
            virtual std::string GetContainerType() override;
            //!
            //! @brief Publish topic to mqtt broker
            //!
            //! @param topic Topic to be published
            //! @param value Value published to topic
            //! @return true Publish was successfull
            //! @return false Publis wasn't successfull
            //!
            bool publish(std::string topic, std::string value);
    };
} // namespace ModelController
