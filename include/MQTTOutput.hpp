//!
//! @file MQTTOutput.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of a MQTT-Output-Variable (published to MQTT broker)
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseOutputConnector.hpp"
#include "IMQTTOutput.hpp"
#include <sstream>

namespace ModelController
{
    template <typename T>
    class MQTTOutput : public BaseOutputConnector<T>, public IMQTTOutput
    {
    private:
        //!
        //! @brief Function pointer to publish function of MQTT client
        //!
        std::function<bool(std::string, std::string)> publish;
    protected:
        //!
        //! @brief Publish value as string to MQTT broker
        //!
        //! @param value Value to be send to broker
        //! @return true Setting of value succeeded
        //! @return false Setting of value failed
        //!
        virtual bool SetOutputValue(T value)
        {
            std::ostringstream mqttValue;
            mqttValue << value;
            return publish("/" + this->GetName(), mqttValue.str());
        }
    public:
        //!
        //! @brief Construct a new MQTTOutput object
        //!
        //! @param name Name of the connector
        //! @param publish Function pointer to publish function of MQTT client
        //! @param pathConnectedModuleOut Path of the connected output
        //! @param parent Parent of the Connector (normally pass this)
        //!
        MQTTOutput(std::string name, std::function<bool(std::string, std::string)> publish, std::string pathConnectedModuleOut = "", BaseConnector* parent = nullptr)
            : BaseOutputConnector<T>(name, pathConnectedModuleOut, parent),
            publish(publish)
        {
        }
    };
} // namespace ModelController
