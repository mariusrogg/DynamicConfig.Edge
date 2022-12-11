#pragma once
#include "HWDevice.hpp"

namespace ModelController
{
    class BaseAnalogueOutput : public HWDevice
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
        //! @param name Name of the device
        //! @param config Config of the device
        //! @param parent Parent of the HWDevice (normally pass this)
        //!
        BaseAnalogueOutput(std::string name, JsonObject config, HWDevice* parent = nullptr);
        //!
        //! @brief Destroy the Base Analog Output object
        //!
        ~BaseAnalogueOutput();
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
