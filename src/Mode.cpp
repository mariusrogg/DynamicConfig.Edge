#include "Mode.hpp"
#include <sstream>

//!
//! @brief Construct new mode with json config
//!
Mode::Mode(JsonObject mode)
    : sequence(mode["mode"].is<string>() ? mode["mode"].as<string>() : "{}")
{
    on = mode["on"].is<bool>() ? mode["on"] : false;
    off = mode["off"].is<bool>() ? mode["off"] : false;
    syncTime = mode["sync"].is<long>() ? mode["sync"] : -1;
}
//!
//! @brief Destruction of the Mode object
//!
Mode::~Mode()
{
}
//!
//! @brief Get actual value of the sequence for specified time
//!
double Mode::GetValue(unsigned long time)
{
    return sequence.GetValue(time);
}
//!
//! @brief Get sync time
//!
long Mode::GetSyncTime() const
{
    return syncTime;
}
//!
//! @brief Returns on
//!
bool Mode::GetOn() const
{
    return on;
}
//!
//! @brief Returns off
//!
bool Mode::GetOff() const
{
    return off;
}
//!
//! @brief Build config as json depending on values
//!
string Mode::GetConfig()
{
    stringstream config;
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