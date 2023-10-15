//!
//! @file IMQTTInput.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for MQTTInput-Interface
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <string>
#include "BaseConnector.hpp"

namespace ModelController
{
    class IMQTTInput
    {
    public:
        //!
        //! @brief Construct a new IMQTTInput object
        //!
        IMQTTInput() {}
        //!
        //! @brief Set actual value of the input variable
        //!
        //! @param value Value to be set
        //!
        virtual void SetMQTTValue(std::string value) = 0;
        //!
        //! @brief Get the name of the object
        //!
        //! @return std::string Name of the object
        //!
        virtual std::string GetName() const = 0;
    };
} // namespace ModelController
