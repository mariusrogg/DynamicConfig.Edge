//!
//! @file ProcessorIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Input variables for processor-modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModuleIn.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    template <typename T>
    class ProcessorIn : public BaseModuleIn<T>
    {
        private:
            //!
            //! @brief Listener called, if input value changed
            //!
            typename Event<T>::Listener inputChanged;
            //!
            //! @brief Get the Path Connected Module Out object out of the config
            //!
            //! @param name Name of the actual object (ProcessorIn)
            //! @param parentConfig Json config of the parent object or json string with path to connected output module
            //! @return std::string Extracted path of the connected output module, empty string if not found in parentConfig
            //!
            std::string GetPathConnectedModuleOut(std::string name, JsonVariant parentConfig)
            {
                std::string path;
                Logger::trace("ProcessorIn::GetPathConnectedModuleOut(" + name + ", " + parentConfig.as<std::string>() + ")");
                if (parentConfig.is<JsonObject>())
                {
                    Logger::trace("Parent config is JsonObject");
                    if (parentConfig[name].is<std::string>())
                    {
                        Logger::trace("Key is set in config");
                        path = parentConfig[name].as<std::string>();
                    }
                }
                else if (parentConfig.is<std::string>())
                {
                    Logger::trace("Parent config is string");
                    path = parentConfig.as<std::string>();
                }
                Logger::trace("Path of connected module out is: " + path + " - size: " + std::to_string(path.size()));
                return path;
            }
        protected:
            //!
            //! @brief Method not needed for Processors, since nothing more than actualValue needs to be set, returns always true
            //!
            virtual bool SetInputValue(T value) override
            {
                return true;
            }
        public:
            //!
            //! @brief Construct a new Processor In object
            //!
            //! @param name Name of the actual object (ProcessorIn)
            //! @param parentConfig Json config of the parent object or json string with path to connected output module
            //! @param inputChanged Function called, if input changed
            //! @param parent Parent of the ProcessorIn (normally pass this)
            //!
            ProcessorIn(std::string name, JsonVariant parentConfig, std::function<void(T)> onInputChanged, BaseModule* parent = nullptr)
                : BaseModuleIn<T>(name, GetPathConnectedModuleOut(name, parentConfig), parent),
                inputChanged(&(this->ValueChangedEvent), onInputChanged)
            {
                if (this->pathConnectedModuleOut.empty())
                {
                    this->pathConnectedModuleOut = "/root/Connectors/mqtt" + this->GetShortPath();
                    Logger::trace("Creating connection to default connector for " + this->GetShortPath());
                    this->OnOutputCreated(this->pathConnectedModuleOut);
                }
            }
    };

} // namespace ModelController

