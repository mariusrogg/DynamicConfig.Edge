//!
//! @file OnboardPWM.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implemantation of the Onboard PWM
//!
//! @copyright Copyright (c) 2023
//!
#include "OnboardPWM.hpp"
#include <sstream>
#include "Logger.hpp"

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
        Logger::trace("Channel: " + std::to_string(channel));
        Logger::trace("Resolution: " + std::to_string(resolution));
        Logger::trace("Frequency: " + std::to_string(frequency));
        this->resolution = resolution;
        this->frequency = frequency;
        if (channel >= 0)
        {
            ledcSetup(channel, frequency, resolution);
            for (std::vector<uint8_t>::iterator it = pins.begin(); it != pins.end(); it++)
            {
                Logger::trace("pin: " + std::to_string(*it));
                ledcDetachPin(*it);
                ledcAttachPin(*it, channel);
            }
        }
    }
    //!
    //! @brief Set output value to the PWM
    //!
    bool OnboardPWM::SetValue(double value)
    {
        Logger::trace("Set value " + std::to_string(value) + " to " + GetPath());
        bool retVal = false;
        uint32_t duty = value / 100 * (pow(2, resolution.GetValue()) - 1);
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
    OnboardPWM::OnboardPWM(std::string name, JsonObject config, BaseModule* parent)
        : BaseContainer(name, config, parent),
        in("in", config, [&](double value) { this->SetValue(value); }, this),
        resolution("resolution", config, 16, this),
        frequency("frequency", config, 500, this)
    {
        Logger::trace("OnboardPWM::OnboardPWM(" + name + ", jsonConfig,  " + (parent == nullptr ? "NULL" : parent->GetPath()) + ")");

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
                    this->pins.push_back(it->as<uint8_t>());
                    Logger::trace("Pin " + std::to_string(it->as<uint8_t>()) + " added to OnboardPWM " + GetPath());
                }
            }
        }
        SetChannel(frequency, resolution);
    }
    //!
    //! @brief Destruction of the Onboard PWM object
    //!
    OnboardPWM::~OnboardPWM()
    {
        SetValue(0);
        usedChannels.erase(remove(usedChannels.begin(), usedChannels.end(), channel), usedChannels.end());
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
        config << ", \"resolution\": " << resolution;
        config << ", \"frequency\": " << frequency;
        return config.str();
    }
} // namespace ModelController
