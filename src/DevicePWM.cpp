#include "DevicePWM.hpp"
#include <sstream>

//!
//! @brief Calcultes time start on-sequence depending on duration of on-sequence and on flag of active mode
//!
long DevicePWM::GetTimeStartOn() const
{
    long timeStartOn = timeStartMode;
    // On sequence is executed directly before mode
    if (activeMode != nullptr && activeMode->GetOn())
    {
        timeStartOn -= on.GetDuration();
    }
    return timeStartOn;
}
//!
//! @brief Calcultes time end off-sequence depending on duration of off-sequence and off flag of active mode
//!
long DevicePWM::GetTimeEndOff() const
{
    long timeEndOff = timeEndMode;
    // Calculate end time of off sequence
    if (activeMode != nullptr && activeMode->GetOff())
    {
        timeEndOff += off.GetDuration();
    }
    return timeEndOff;
}
//!
//! @brief Get mode by name
//!
Mode* DevicePWM::GetMode(string name)
{
    Mode* mode = nullptr;
    std::map<string, Mode*>::iterator pos = modes.find(name);
    if (pos != modes.end())
    {
        mode = (pos->second);
    }
    return mode;
}
//!
//! @brief Initializing modes, sequences, ... by json
//!
DevicePWM::DevicePWM(JsonObject deviceConfig)
    : channel(deviceConfig["pins"].as<JsonObject>()),
    defaultMode(deviceConfig["defaultMode"].as<string>()),
    off(deviceConfig["off"].is<string>() ? deviceConfig["off"].as<string>() : "{}"),
    on(deviceConfig["on"].is<string>() ? deviceConfig["on"].as<string>() : "{}")
{
    JsonObject jsonModes = deviceConfig["modes"].as<JsonObject>();
    for (JsonObject::iterator it = jsonModes.begin(); it != jsonModes.end(); ++it)
    {
        if (it->value().is<JsonObject>())
        {
            modes[it->key().c_str()] = new Mode(it->value().as<JsonObject>());
        }
    }
    activeMode = GetMode(defaultMode);
}
//!
//! @brief Nothing to destruct
//!
DevicePWM::~DevicePWM()
{
    for (std::map<string, Mode*>::iterator it = modes.begin(); it != modes.end(); ++it)
    {
        if (it->second != nullptr)
        {
            delete it->second;
        }
    }
}
//!
//! @brief Set target value for PWM-Device (mode, manual value, active)
//!
void DevicePWM::SetTarget(JsonVariant targetValue)
{
    manualTarget = -1;
    if (targetValue.is<bool>())
    {
        active = targetValue.as<bool>();
    }
    else if (targetValue.is<double>())
    {
        manualTarget = targetValue.as<double>();
    }
    else if (targetValue.is<string>())
    {
        Mode* targetMode = GetMode(targetValue.as<string>());
        if (targetMode != activeMode)
        {
            nextActiveMode = targetMode;
        }
        if (nextActiveMode == nullptr)
        {
            active = false;
        }
        else
        {
            active = true;
        }
    }
}
//!
//! @brief Calculate target value and set value to channel
//!
void DevicePWM::Execute()
{
    double value = manualTarget;
    if (value < 0)
    {
        if (activeMode == nullptr)
        {
            activeMode = nextActiveMode;
            nextActiveMode = nullptr;
        }
        value = 0;
        // ToDo: Duration -1
        // End if not active or no active mode is set or new next active mode is set
        // End time is time where next sync cycle is finished (if used) plus duration of off-sequence (if used)
        if (!active || activeMode == nullptr || (activeMode != nextActiveMode && nextActiveMode != nullptr))
        {
            // If time for ending was set already or time started was not set yet, time for ending does not need to be set
            if (timeEndMode < 0 && timeStartMode >= 0)
            {
                timeEndMode = millis();
                // Earliest time for ending is time where mode was started
                if (timeEndMode < timeStartMode)
                {
                    timeEndMode = timeStartMode;
                }
                // End mode only after end of sync cycle, if sync is active
                if (activeMode != nullptr && activeMode->GetSyncTime() >= 0)
                {
                    long syncTime = activeMode->GetSyncTime();
                    if (timeEndMode % syncTime != 0)
                    {
                        timeEndMode = (timeEndMode / syncTime) * syncTime + syncTime;
                    }
                }
            }
            // if actual time is bigger then time where mode ends, time for starting mode and time for starting on sequence is not needed anymore
            if (millis() > timeEndMode && timeEndMode >= 0)
            {
                timeStartMode = -1;
            }
        }
        else
        {
            if (timeStartMode < 0)
            {
                timeStartMode = millis();
                // Earliest time for starting is time where off-sequence is ending
                if (timeStartMode < GetTimeEndOff())
                {
                    timeStartMode = GetTimeEndOff();
                }
                if (activeMode != nullptr)
                {
                    // Mode can only be started after on-sequence (if set)
                    if (activeMode->GetOn())
                    {
                        timeStartMode += on.GetDuration();
                    }
                    // Mode is only started with beginning of sync cycle (if set)
                    if (activeMode->GetSyncTime() >= 0)
                    {
                        unsigned long endWithoutSync = timeStartMode;
                        unsigned long endWithSync = (endWithoutSync / activeMode->GetSyncTime()) * activeMode->GetSyncTime();
                        if (endWithoutSync % activeMode->GetSyncTime() != 0)
                        {
                            endWithSync += activeMode->GetSyncTime();
                        }
                        timeStartMode = endWithSync;
                    }
                }
            }
            // if actual time is bigger then time where mode starts, time for ending mode and time for ending off sequence is not needed anymore
            if (millis() > timeStartMode && timeStartMode >= 0)
            {
                timeEndMode = -1;
            }
        }

        if (activeMode != nullptr)
        {
            unsigned long actualTime = millis();
            if (timeStartMode >= 0 && actualTime >= GetTimeStartOn() && actualTime <= timeStartMode)
            {
                // Run on sequence
                value = on.GetValue(actualTime - GetTimeStartOn());
            }
            else if (timeStartMode >= 0 && actualTime >= timeStartMode && (timeEndMode < 0 || actualTime <= timeEndMode))
            {
                // Run Mode
                value = activeMode->GetValue(actualTime - timeStartMode);
            }
            else if (timeEndMode >= 0 && actualTime >= timeEndMode && actualTime <= GetTimeEndOff())
            {
                // Run off sequence
                value = off.GetValue(actualTime - timeEndMode);
            }
            else if (timeEndMode >= 0 && actualTime >= GetTimeEndOff())
            {
                // Off
                value = 0;
            }
        }
    }
    else
    {
        active = false;
        timeStartMode = -1;
        timeEndMode = -1;
    }
    channel.SetValue(value);
}
//!
//! @brief Build config of Device
//!
string DevicePWM::GetConfig()
{
    stringstream config;
    config << "\"type\": \"" << type << "\",";
    config << "\"pins\": {" << channel.GetConfig() << "},";
    config << "\"defaultMode\": \"" << defaultMode << "\",";
    config << "\"on\": \"" << on.GetSequenceString() << "\",";
    config << "\"off\": \"" << off.GetSequenceString() << "\",";
    config << "\"modes\": {";
    for (std::map<string, Mode*>::iterator it = modes.begin(); it != modes.end(); it++)
    {
        if (it != modes.begin())
        {
            config << ",";
        }
        config << "\"" << it->first << "\": {" << it->second->GetConfig() << "}";
    }
    config << "}";
    return config.str();
}