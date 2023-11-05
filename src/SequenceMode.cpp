//!
//! @file SequenceMode.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the SequenceMode
//!
//! @copyright Copyright (c) 2023
//!
//!
#include "SequenceMode.hpp"
#include <sstream>

namespace ModelController
{
    //!
    //! @brief Construct new mode with json config
    //!
    SequenceMode::SequenceMode(JsonObject mode)
        : sequence(mode["mode"].is<std::string>() ? mode["mode"].as<std::string>() : "{}")
    {
        on = mode["on"].is<bool>() ? mode["on"] : false;
        off = mode["off"].is<bool>() ? mode["off"] : false;
        syncTime = mode["sync"].is<long>() ? mode["sync"] : -1;
    }
    //!
    //! @brief Destruction of the SequenceMode object
    //!
    SequenceMode::~SequenceMode()
    {
    }
    //!
    //! @brief Get actual value of the sequence for specified time
    //!
    double SequenceMode::GetValue(unsigned long time)
    {
        return sequence.GetValue(time);
    }
    //!
    //! @brief Get sync time
    //!
    long SequenceMode::GetSyncTime() const
    {
        return syncTime;
    }
    //!
    //! @brief Returns on
    //!
    bool SequenceMode::GetOn() const
    {
        return on;
    }
    //!
    //! @brief Returns off
    //!
    bool SequenceMode::GetOff() const
    {
        return off;
    }
    //!
    //! @brief Build config as json depending on values
    //!
    std::string SequenceMode::GetConfig()
    {
        std::stringstream config;
        config << "\"mode\": \"" + sequence.GetSequenceString() + "\"";
        if (on)
        {
            config << ",\"on\": true";
        }
        if (off)
        {
            config << ",\"off\": true";
        }
        if (syncTime >= 0)
        {
            config << ",\"off\": " << syncTime;
        }
        return config.str();
    }
} //namespace ModelController