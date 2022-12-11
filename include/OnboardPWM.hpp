#pragma once
#include "BaseAnalogueOutput.hpp"
#include <vector>
#include <map>

namespace ModelController
{
    class OnboardPWM : BaseAnalogueOutput
    {
    private:
        //!
        //! @brief PWM-Channels already used by the program
        //!
        static std::vector<int> usedChannels;
        //!
        //! @brief Amount of channels on the controller
        //!
        static constexpr int numberChannels = 16;
        //!
        //! @brief Number of the channel
        //!
        int channel;
        //!
        //! @brief Resolution of the channel
        //!
        int resolution;
        //!
        //! @brief Pins attached to the channel
        //!
        std::vector<int> pins;
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
        void SetChannel(int frequency = DefaultFrequency, int resolution = DefaultResolution);

    protected:
        //!
        //! @brief Set value of the pwm output
        //!
        //! @param value Value to be set to output in percent
        //! @return true Value was successfully set to output
        //! @return false Value could not be set to output
        //!
        virtual bool SetOutputValue(double value) = 0;

    public:
        //!
        //! @brief Name of the type for config
        //!
        static constexpr const char* type = "pwm";
        //!
        //! @brief Default cycle frequency of the channel
        //!
        static const int DefaultFrequency = 5000;
        //!
        //! @brief Default resolution of the channel
        //!
        static const int DefaultResolution = 8;
        //!
        //! @brief Construct a new PWMChannel object
        //!
        //! @param name Name of the device
        //! @param config Config of the device
        //! @param parent Parent of the HWDevice (normally pass this)
        //!
        OnboardPWM(std::string name, JsonObject config, HWDevice* parent = nullptr);
        //!
        //! @brief Destroy the PWMChannel object
        //!
        ~OnboardPWM();
        //!
        //! @brief Generate the config of the device
        //!
        //! @param doc Json document to create Config to
        //!
        virtual string GetConfig(JsonDocument* doc) override;
    };
} // namespace ModelController
