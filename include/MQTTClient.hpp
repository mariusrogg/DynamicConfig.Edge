//!
//! @file MQTTClient.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Wrapper for MQTTClient
//!
//! @copyright Copyright (c) 2023
//!
#pragma once

#include "PubSubClient.h"
#include "BaseConnector.hpp"
#include "WiFi.h"
#include <map>
#include "MQTTInput.hpp"
#include "IMQTTInput.hpp"
#include "MQTTOutput.hpp"
#include "LoopEvent.hpp"

namespace ModelController
{
    class MQTTClient : public BaseConnector
    {
        private:
            //!
            //! @brief Listener for periodic task
            //!
            LoopEvent::LoopListener loopListener;
            //!
            //! @brief List of input variables
            //!
            std::map<std::string, IMQTTInput*> inputVariables;
            //!
            //! @brief Default hostname of the MQTT broker
            //!
            static constexpr const char* DefaultServer = "raspberrypi";
            //!
            //! @brief Hostname of the MQTT broker
            //!
            std::string serverHostname;
            //!
            //! @brief Default MQTT brokers port
            //!
            static const uint32_t DefaultPort = 1883;
            //!
            //! @brief MQTT brokers port
            //!
            uint16_t serverPort;
            //!
            //! @brief ClientID (name of the client)
            //!
            std::string clientID;
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
            //! @param type ConnectorType of the child
            //! @param dataType ConnectorDataType of the child
            //! @return BaseConnector* Child, nullptr if not found
            //!
            virtual BaseConnector* GetChild(std::string connectorPath, ConnectorType type = ConnectorType::eUndefined, ConnectorDataType dataType = ConnectorDataType::eUndefined) override;
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
            MQTTClient(std::string name, JsonObject config, BaseConnector* parent = nullptr);
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
