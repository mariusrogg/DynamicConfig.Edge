//!
//! @file BaseAnalogueOutput.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base-Class for analogue output values
//! @version 0.1.0
//! @date 2023-09-14
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "Connector.hpp"

namespace ModelController
{
    class BaseAnalogueOutput : public Connector
    {
    private:
        //!
        //! @brief Actual value (set to output)
        //!
        double actualValue = 0;

    protected:
        //!
        //! @brief Set value of the analog output (e. g. 0 to 10 V, 0 to 100% in PWM, ...)
        //!
        //! @param value Value to be set to output in percent
        //! @return true Value was successfully set to output
        //! @return false Value could not be set to output
        //!
        virtual bool SetOutputValue(double value) = 0;

    public:
        //!
        //! @brief Construct a new Base Analog Output object
        //!
        //! @param name Name of the analogue output
        //! @param config Config of the analogue output
        //! @param parent Parent of the Connector (normally pass this)
        //!
        BaseAnalogueOutput(std::string name, JsonObject config, Connector* parent = nullptr);
        //!
        //! @brief Destroy the Base Analog Output object
        //!
        virtual ~BaseAnalogueOutput();
        //!
        //! @brief Set the channels duty value
        //!
        //! @param value Duty value
        //!
        void SetValue(double value);
        //!
        //! @brief Get the actual value set to channel
        //!
        //! @return double Actual value set to channel
        //!
        double GetValue() const;
    };
} // namespace ModelController
