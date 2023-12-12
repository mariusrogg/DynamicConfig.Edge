//!
//! @file ProcessorOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Output variables for processor-modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModuleOut.hpp"

namespace ModelController
{
    template <typename T>
    class ProcessorOut : public BaseModuleOut<T>
    {
        private:
            //!
            //! @brief Get the Path Connected Module In object out of the config
            //!
            //! @param name Name of the actual object (ProcessorOut)
            //! @param parentConfig Json config of the parent object or json string with path to connected input module
            //! @return std::string Extracted path of the connected input module, empty string if not found in parentConfig
            //!
            static std::string GetPathConnectedModuleIn(std::string name, JsonVariant parentConfig)
            {
                std::string path;
                Logger::trace("ProcessorOut::GetPathConnectedModuleIn(" + name + "," + parentConfig.as<std::string>() + ")");
                if (parentConfig.is<JsonObject>())
                {
                    Logger::trace("Parent config is JsonObject");
                    if (parentConfig[name.c_str()].is<std::string>())
                    {
                        Logger::trace("Key is set in config");
                        path = parentConfig[name.c_str()].as<std::string>();
                    }
                }
                else if (parentConfig.is<std::string>())
                {
                    Logger::trace("Parent config is string");
                    path = parentConfig.as<std::string>();
                }
                Logger::trace("Path of connected module in is: " + path);
                return path;
            }
        public:
            //!
            //! @brief Construct a new Processor In object
            //!
            //! @param name Name of the actual object (ProcessorIn)
            //! @param parentConfig Json config of the parent object or json string with path to connected output module
            //! @param parent Parent of the ProcessorIn (normally pass this)
            //!
            ProcessorOut(std::string name, JsonVariant parentConfig, BaseModule* parent = nullptr)
                : BaseModuleOut<T>(name, GetPathConnectedModuleIn(name, parentConfig), parent)
            {
                if (this->pathConnectedModuleIn.empty())
                {
                    this->pathConnectedModuleIn = "/root/Connectors/mqtt" + this->GetShortPath();
                    Logger::trace("Creating connection to default connector for " + this->GetShortPath());
                    this->OnInputCreated(this->pathConnectedModuleIn);
                }
            }
    };

} // namespace ModelController

