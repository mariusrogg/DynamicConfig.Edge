//!
//! @file OnboardPWM.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implemantation of the Onboard PWM
//!
//! @copyright Copyright (c) 2023
//!
#include "OnboardPWM.hpp"
#include <sstream>
namespace ModelController
{
    //!
    //! @brief List of already used channels
    //!
    std::vector<uint8_t> OnboardPWM::usedChannels;
    //!
    //! @brief Get next free channel
    //!
    int OnboardPWM::GetFreeChannel()
    {
        int channel = -1;
        for (int i = 0; i < numberChannels - 1 && channel < 0; i++)
        {
            if (std::find(usedChannels.begin(), usedChannels.end(), i) == usedChannels.end())
            {
                channel = i;
            }
        }
        return channel;
    }
    //!
    //! @brief Set config for the channel
    //!
    void OnboardPWM::SetChannel(uint32_t frequency, uint8_t resolution)
    {
        this->resolution = resolution;
        this->frequency = frequency;
        if (channel >= 0)
        {
            ledcSetup(channel, frequency, resolution);
            for (std::vector<uint8_t>::iterator it = pins.begin(); it != pins.end(); it++)
            {
                ledcDetachPin(*it);
                ledcAttachPin(*it, channel);
            }
            Serial.println();
        }
    }
    //!
    //! @brief Set output value to the PWM
    //!
    bool OnboardPWM::SetOutputValue(double value)
    {
        bool retVal = false;
        uint32_t duty = value / 100 * (pow(2, resolution) - 1);
        if (channel >= 0)
        {
            ledcWrite(channel, duty);
            retVal = true;
        }
        return retVal;
    }
    //!
    //! @brief Construct a new Onboard PWM object
    //!
    OnboardPWM::OnboardPWM(std::string name, JsonObject config, BaseConnector* parent)
        : BaseOutputConnector<double>(name, config, parent)
    {
        channel = GetFreeChannel();
        if (channel >= 0)
        {
            usedChannels.push_back(channel);
        }
        JsonVariant jPins = config["pin"];
        if (jPins.is<JsonArray>())
        {
            JsonArray pins = jPins.as<JsonArray>();
            for (JsonArray::iterator it = pins.begin(); it != pins.end(); ++it)
            {
                if (it->is<uint8_t>())
                {
                    Serial.println(it->as<uint8_t>());
                    this->pins.push_back(it->as<uint8_t>());
                }
            }
        }
        JsonVariant jFrequency = config["frequency"];
        uint32_t frequency = DefaultFrequency;
        if (jFrequency.is<uint8_t>())
        {
            frequency = jFrequency.as<uint32_t>();
        }
        JsonVariant jResolution = config["resolution"];
        uint8_t resolution = DefaultResolution;
        if (jResolution.is<uint8_t>())
        {
            resolution = jResolution.as<uint8_t>();
        }
        SetChannel(frequency, resolution);
    }
    //!
    //! @brief Destruction of the Onboard PWM object
    //!
    OnboardPWM::~OnboardPWM()
    {
        SetValue(0);
        for (std::vector<uint8_t>::iterator it = pins.begin(); it != pins.end(); it++)
        {
            ledcDetachPin(*it);
        }
    }
    //!
    //! @brief Returns config of the PWM-Channel
    //!
    std::string OnboardPWM::GetConfig()
    {
        std::stringstream config;
        config << "\"pins\": [";
        for (std::vector<uint8_t>::iterator it = pins.begin(); it != pins.end(); it++)
        {
            if (it != pins.begin())
            {
                config << ", ";
            }
            config << *it;
        }
        config << "]";
        config << ", \"type\": \"" << type << "\"";
        if (resolution != DefaultResolution)
        {
            config << ", \"resolution\": " << resolution;
        }
        if (frequency != DefaultFrequency)
        {
            config << ", \"frequency\": " << frequency;
        }
        return config.str();
    }
} // namespace ModelController
