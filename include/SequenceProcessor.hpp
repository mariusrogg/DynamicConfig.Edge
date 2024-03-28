//!
//! @file SequenceProcessor.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class processing sequences
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseContainer.hpp"
#include "ModuleIn.hpp"
#include "ModuleOut.hpp"
#include "Sequence.hpp"
#include <map>
#include "SequenceMode.hpp"
#include "LoopEvent.hpp"
#include "ConfigItem.hpp"

namespace ModelController
{
    class SequenceProcessor : public BaseContainer
    {
        private:
            //!
            //! @brief Listener called periodically
            //!
            LoopEvent::LoopListener loopListener;
            //!
            //! @brief Output value of the Sequence processor
            //!
            ModuleOut<double> out;
            //!
            //! @brief Input value for setting active
            //!
            ModuleIn<bool> activate;
            //!
            //! @brief Method called, if activate value changed
            //!
            //! @param value Value set to activate
            //!
            void OnActivateChanged(bool value);
            //!
            //! @brief Input value for setting output manually
            //!
            ModuleIn<double> manualTarget;
            //!
            //! @brief Method called, if manualTarget value changed
            //!
            //! @param value Value set to manualTarget
            //!
            void OnManualTargetChanged(double value);
            //!
            //! @brief Input value for setting target mode
            //!
            ModuleIn<std::string> targetMode;
            //!
            //! @brief Method called, if targetMode value changed
            //!
            //! @param value Value set to targetMode
            //!
            void OnTargetModeChanged(std::string value);
            //!
            //! @brief Possible modes
            //!
            std::map<std::string, SequenceMode*> modes; // ToDo: ConfigItem
            //!
            //! @brief Name of the default mode
            //!
            ConfigItem<std::string> defaultMode;
            //!
            //! @brief Active mode
            //!
            SequenceMode* activeMode = nullptr;
            //!
            //! @brief Next activated mode
            //!
            SequenceMode* nextActiveMode = nullptr;
            //!
            //! @brief Sequence on turning mode off
            //!
            Sequence off;   // ToDo: ConfigItem
            //!
            //! @brief Sequence on turning mode on
            //!
            Sequence on;
            //!
            //! @brief Time where mode was started
            //!
            long timeStartMode = -1;
            //!
            //! @brief Get the time where on sequence is started
            //!
            //! @return long Time where on sequence is started
            //!
            long GetTimeStartOn() const;
            //!
            //! @brief Time where mode was set to end
            //!
            long timeEndMode = -1;
            //!
            //! @brief Get the time where off sequence is ended
            //!
            //! @return long Time where off sequence is ended
            //!
            long GetTimeEndOff() const;
            //!
            //! @brief True if mode is active
            //!
            ModuleOut<bool> active;
            //!
            //! @brief True if mode should be active in next cycle
            //!
            bool targetActive = false;
            //!
            //! @brief Get the mode by its name
            //!
            //! @param name Name of the mode
            //! @return Mode* Mode with specified name
            //!
            SequenceMode* GetMode(std::string name);
            //!
            //! @brief Manual set target value, -1 if none is set
            //!
            double manualSetpoint = -1;
            //!
            //! @brief Execute channel logic to calculate output
            //!
            void Execute();

        public:
            //!
            //! @brief Name of the type for config
            //!
            static constexpr const char* type = "sequence";
            //!
            //! @brief Construct a new Sequence Processor object
            //!
            //! @param name Name of the processor
            //! @param config Config of the processor
            //! @param parent Parent of the Processor (normally pass this)
            //!
            SequenceProcessor(std::string name, JsonObject config, BaseModule* parent = nullptr);
            //!
            //! @brief Destruction of the Sequence Processor object
            //!
            ~SequenceProcessor();
            //!
            //! @brief Get the type of the container
            //!
            //! @return std::string Type of the container in string representation
            //!
            virtual std::string GetContainerType() override;
    };
} // namespace ModelController
