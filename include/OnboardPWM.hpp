//!
//! @file OnboardPWM.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for controlling onboard PWM
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseContainer.hpp"
#include "ModuleIn.hpp"
#include <vector>
#include <map>
#include "ConfigItem.hpp"

namespace ModelController
{
    class OnboardPWM : public BaseContainer
    {
    private:
        ModuleIn<double> in;
        //!
        //! @brief PWM-Channels already used by the program
        //!
        static std::vector<uint8_t> usedChannels;
        //!
        //! @brief Amount of channels on the controller
        //!
        static constexpr uint8_t numberChannels = 16;
        //!
        //! @brief Number of the channel
        //!
        uint8_t channel;
        //!
        //! @brief Resolution of the channel
        //!
        ConfigItem<uint8_t> resolution;
        //!
        //! @brief Frequency of the channel
        //!
        ConfigItem<uint32_t> frequency;
        //!
        //! @brief Pins attached to the channel
        //!
        std::vector<uint8_t> pins;  // ToDo: Config Item for Array
        //!
        //! @brief Get the a free channel
        //!
        //! @return int Free channel
        //!
        static int GetFreeChannel();
        //!
        //! @brief Reset the channels parameters
        //!
        //! @param frequency   Cycle Frequency of the channel
        //! @param resolution  Resolution of the channel
        //!
        void SetChannel(uint32_t frequency, uint8_t resolution);

    protected:
        //!
        //! @brief Set value of the pwm output
        //!
        //! @param value Value to be set to output in percent
        //! @return true Value was successfully set to output
        //! @return false Value could not be set to output
        //!
        bool SetValue(double value);

    public:
        //!
        //! @brief Name of the type for config
        //!
        static constexpr const char* type = "pwm";
        //!
        //! @brief Construct a new PWMChannel object
        //!
        //! @param name Name of the connector
        //! @param config Config of the connector
        //! @param parent Parent of the Connector (normally pass this)
        //!
        OnboardPWM(std::string name, JsonObject config, BaseModule* parent = nullptr);
        //!
        //! @brief Destroy the PWMChannel object
        //!
        virtual ~OnboardPWM();
        //!
        //! @brief Get the config of the connector
        //!
        //! @return string Config created
        //!
        virtual std::string GetConfig() override;
    };
} // namespace ModelController
