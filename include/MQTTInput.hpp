//!
//! @file MQTTInput.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of a MQTT-Input-Variable (subscribed from MQTT broker)
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseInputConnector.hpp"
#include "IMQTTInput.hpp"
#include <sstream>
#include <functional>

namespace ModelController
{
    template <typename T>
    class MQTTInput : public IMQTTInput, public BaseInputConnector<T>
    {
    public:
        //!
        //! @brief Construct a new MQTTInput object
        //!
        //! @param name Name of the variable (topic without slash at beginning)
        //! @param parent Parent of the MQTTInput (MQTTClient)
        //!
        MQTTInput(std::string name, BaseConnector* parent = nullptr)
            : BaseInputConnector<T>(name, parent)
        {
        }
        //!
        //! @brief Set incoming message value, convert to T and set to input variable
        //!
        //! @param value value to be set to input variable
        //!
        virtual void SetMQTTValue(std::string value)
        {
            std::stringstream convert(value);
            T val;
            convert >> val;
            this->SetValue(val);
        }
        //!
        //! @brief Override GetName from IMQTTInput
        //!
        //! @return std::string Name of the object
        //!
        virtual std::string GetName() const override
        {
            return BaseInputConnector<T>::GetName();
        }
    };
} // namespace ModelController
