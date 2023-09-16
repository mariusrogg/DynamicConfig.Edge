//!
//! @file BaseAnalogueOutput.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the analog output base class
//! @version 0.0.0
//! @date 2023-09-14
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseAnalogueOutput.hpp"

namespace ModelController
{
    //!
    //! @brief Construct a new Base Analogue Output object
    //!
    BaseAnalogueOutput::BaseAnalogueOutput(std::string name, JsonObject config, Connector* parent)
        : Connector(name, config, parent)
    {
    }
    //!
    //! @brief Destruction of the Base Analogue Output
    //!
    BaseAnalogueOutput::~BaseAnalogueOutput()
    {
    }
    //!
    //! @brief Set target value for the output
    //!
    void BaseAnalogueOutput::SetValue(double value)
    {
        if (SetOutputValue(value))
        {
            actualValue = value;
        }
    }
    double BaseAnalogueOutput::GetValue() const
    {
        return actualValue;
    }
} // namespace ModelController
