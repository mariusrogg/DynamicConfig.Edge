//!
//! @file SequenceProcessor.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the SequenceProcessor
//!
//! @copyright Copyright (c) 2023
//!
#include "SequenceProcessor.hpp"
#include <sstream>
namespace ModelController
{
    //!
    //! @brief Set active value
    //!
    void SequenceProcessor::OnActivateChanged(bool value)
    {
        if (value)
        {
            manualSetpoint = -1;
        }
        active = value;
    }
    //!
    //! @brief Set manual setpoint value
    //!
    void SequenceProcessor::OnManualTargetChanged(double value)
    {
        manualSetpoint = value;
    }
    //!
    //! @brief Set manual target value
    //!
    void SequenceProcessor::OnTargetModeChanged(std::string value)
    {
        Logger::debug("Setting target mode " + value + " to SequenceProcessor " + GetPath());
        manualSetpoint = -1;
        SequenceMode* targetMode = GetMode(value);
        if (targetMode != activeMode)
        {
            nextActiveMode = targetMode;
        }
        if (targetMode == nullptr)
        {
            active = false;
        }
        else
        {
            active = true;
        }
    }
    //!
    //! @brief Calcultes time start on-sequence depending on duration of on-sequence and on flag of active mode
    //!
    long SequenceProcessor::GetTimeStartOn() const
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
    long SequenceProcessor::GetTimeEndOff() const
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
    SequenceMode* SequenceProcessor::GetMode(std::string name)
    {
        SequenceMode* mode = nullptr;
        std::map<std::string, SequenceMode*>::iterator pos = modes.find(name);
        if (pos != modes.end())
        {
            mode = (pos->second);
        }
        return mode;
    }
    //!
    //! @brief Initializing modes, sequences, ... by json
    //!
    SequenceProcessor::SequenceProcessor(std::string name, BaseModule* parent)
        : BaseContainer(name, parent, ModuleType::eNone, ModuleDataType::eNone),
        loopListener([&](){this->Execute();}),
        out("out", this),
        on(ConfigFile::GetConfig<std::string>(GetPath() + "/on").value_or("{}")),   // ToDo: Sequence and modes as BaseModule
        off(ConfigFile::GetConfig<std::string>(GetPath() + "/off").value_or("{}")), // ToDo: Sequence and modes as BaseModule
        activate("activate", [&](bool value) { this->OnActivateChanged(value); }, this),
        manualTarget("manualTarget", [&](double value) { this->OnManualTargetChanged(value); }, this),
        targetMode("targetMode", [&](std::string value) { this->OnTargetModeChanged(value); }, this),
        defaultMode("defaultMode", "", this),
        active("active", this)
    {
        std::optional<JsonObject> optModes = ConfigFile::GetConfig<JsonObject>(GetPath() + "/modes");   // ToDo: Is nullptr correct?
        if (optModes)
        {
            JsonObject jsonModes = optModes.value();
            for (JsonObject::iterator it = jsonModes.begin(); it != jsonModes.end(); ++it)
            {
                if (it->value().is<JsonObject>())
                {
                    modes[it->key().c_str()] = new SequenceMode(it->value().as<JsonObject>());
                }
            }
        }
        activeMode = GetMode(defaultMode);
    }
    //!
    //! @brief Delete modes
    //!
    SequenceProcessor::~SequenceProcessor()
    {
        for (std::map<std::string, SequenceMode*>::iterator it = modes.begin(); it != modes.end(); ++it)
        {
            if (it->second != nullptr)
            {
                delete it->second;
            }
        }
    }
    //!
    //! @brief Returns type
    //!
    std::string SequenceProcessor::GetContainerType()
    {
        return type;
    }
    //!
    //! @brief Calculate target value and set value to channel
    //!
    void SequenceProcessor::Execute()
    {
        double value = manualSetpoint;
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
                            Logger::debug("Setting timeStartMode for " + GetName() + " to " + std::to_string(timeStartMode));
                            Logger::debug("Actual time " + std::to_string(millis()));
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
                    activate.SetValue(false);
                    timeEndMode = -1;
                    if (nextActiveMode != nullptr)
                    {
                        activeMode = nullptr;
                    }
                }
            }
        }
        else
        {
            active = false;
            timeStartMode = -1;
            timeEndMode = -1;
        }
        out.SetValue(value);
    }

} // namespace ModelController