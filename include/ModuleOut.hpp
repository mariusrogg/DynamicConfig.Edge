//!
//! @file ModuleOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for output values for modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"
#include "IModuleOut.hpp"
#include "ModuleIn.hpp"
#include <type_traits>
#include "Utils.hpp"

namespace ModelController
{
    template <typename T>
    class ModuleIn;

    template <typename T>
    class ModuleOut : public IModuleOut
    {
        private:
            //!
            //! @brief Actual value of the output
            //!
            T actualValue; // ToDo: initialize type dependent
            //!
            //! @brief Listener called, if new output module was created
            //!
            Event<std::string>::Listener* OnModuleInCreated = nullptr;
            //!
            //! @brief Get the Path Connected Module In object out of the config
            //!
            //! @param name Name of the actual object (ModuleOut)
            //! @param parentConfig Json config of the parent object or json string with path to connected input module
            //! @return std::string Extracted path of the connected input module, empty string if not found in parentConfig
            //!
            std::string GetPathConnectedModuleIn(std::string name, JsonVariant parentConfig)
            {
                std::string path;
                Logger::trace("ModuleOut::GetPathConnectedModuleIn(" + name + parentConfig.as<std::string>() + ")");
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
        protected:
            //!
            //! @brief Path to the connected output module
            //!
            std::string pathConnectedModuleIn;
            //!
            //! @brief Callback called, if new output was created, to listen to output changed event
            //!
            //! @param pathCreatedInput Path of the created input
            //!
            void OnInputCreated(std::string pathCreatedInput)
            {
                Logger::trace("ModuleOut::OnInputCreated(" + pathCreatedInput + ") - Module: " + this->GetPath());
                bool inputAvailable = pathCreatedInput == pathConnectedModuleIn;

                Logger::trace("wildcard: \t" + IModuleIn::wildcardSuffix);
                Logger::trace("pathCreatedInput: \t" + pathCreatedInput);
                Logger::trace("pathConnectedModuleIn: \t" + pathConnectedModuleIn);

                size_t wildcardSize = IModuleIn::wildcardSuffix.size();
                size_t pathCreatedInputSize = pathCreatedInput.size();
                size_t pathConnectedModuleInSize = pathConnectedModuleIn.size();

                Logger::trace("wildcardSize: \t" + std::to_string(wildcardSize));
                Logger::trace("pathCreatedInputSize: \t" + std::to_string(pathCreatedInputSize));
                Logger::trace("pathConnectedModuleInSize: \t" + std::to_string(pathConnectedModuleInSize));

                //! Check if pathCreatedInput ends with wildcard and pathConnectedModuleIn is child of pathCreatedInput
                if (!inputAvailable
                    && pathConnectedModuleInSize >= pathCreatedInputSize - wildcardSize
                    && pathCreatedInputSize > wildcardSize
                    && pathCreatedInput.compare(pathCreatedInputSize - wildcardSize, wildcardSize, IModuleIn::wildcardSuffix) == 0
                    && pathConnectedModuleIn.compare(0, pathCreatedInputSize - wildcardSize, pathCreatedInput.substr(0, pathCreatedInputSize - wildcardSize)) == 0)
                {
                    inputAvailable = true;
                }

                //! Only set listener, if not set yet
                if (inputAvailable)
                {
                    //! Find connected input
                    ModuleIn<T>* connectedInput = ModuleIn<T>::GetModuleInput(pathConnectedModuleIn);
                    if (connectedInput != nullptr)
                    {
                        connectedInput->SetOutputChangedEvent(&(this->ValueChangedEvent));
                        //! Listen to ValueChangedEvent of connected input and delete OnModuleInCreated, that the listener does not get called anymore
                        delete OnModuleInCreated;
                        OnModuleInCreated = nullptr;
                    }
                    //! Create Listener to OnModuleInCreated event, if no matching connectedInput was found
                    else if (OnModuleInCreated == nullptr)
                    {
                        OnModuleInCreated = new Event<std::string>::Listener(&IModuleIn::ModuleInCreated, [&](std::string path){ this->OnInputCreated(path); } );
                    }
                }
            }
        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Construct a new output module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleIn Path of the connected input
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleOut(std::string name, BaseModule* parent = nullptr)
                : ModuleOut(name, "", parent)
            {
            }
            //!
            //! @brief Construct a new output module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleIn Path of the connected input
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleOut(std::string name, std::string pathConnectedModuleIn, BaseModule* parent = nullptr)
                : IModuleOut(name, parent, GetDataTypeById(typeid(T))),
                pathConnectedModuleIn(pathConnectedModuleIn)
            {
                if (!this->pathConnectedModuleIn.empty())
                {
                    OnInputCreated(this->pathConnectedModuleIn);
                }
                Logger::trace("Raising ModuleOutCreated(" + this->GetPath() + ")");
                ModuleOutCreated(this->GetPath());
                if (this->pathConnectedModuleIn.empty() && !Utils::StartsWith(GetPath(), "/Connectors/mqtt"))
                {
                    this->pathConnectedModuleIn = "/Connectors/mqtt" + GetShortPath();
                    Logger::trace("Creating connection to default connector for " + this->GetShortPath());
                    this->OnInputCreated(this->pathConnectedModuleIn);
                }
            }
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleOut Path of the connected output
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleOut(std::string name, JsonVariant parentConfig, BaseModule* parent = nullptr)
                : ModuleOut<T>(name, GetPathConnectedModuleIn(name, parentConfig), parent)
            {
            }
            //!
            //! @brief Set the target value for the output
            //!
            //! @param value Target value for output
            //!
            void SetValue(T value)
            {
                std::string strValue;
                std::string strActualValue;
                if constexpr (std::is_same<T, std::string>::value)
                {
                    strValue = value;
                    strActualValue = actualValue;
                }
                else
                {
                    strValue = std::to_string(value);
                    strActualValue = std::to_string(actualValue);
                }
                Logger::trace("ModuleOut::SetValue(" + strValue + ")");
                Logger::trace("actualValue" + strActualValue);
                if (actualValue != value)
                {
                    Logger::trace("Changing value");
                    actualValue = value;
                    ValueChangedEvent(GetValue());
                }
            }
            //!
            //! @brief Assignment operator to set value
            //!
            //! @param value Value to be set
            //!
            void operator=(T value)
            {
                SetValue(value);
            }
            //!
            //! @brief Set incoming string value, convert to T and set to variable
            //!
            //! @param value value to be set to variable
            //!
            virtual void SetStringValue(std::string value) override
            {
                this->SetValue(Utils::FromString<T>(value));
            }
            //!
            //! @brief Get the actual value set to output
            //!
            //! @return T Actual value
            //!
            T GetValue() const
            {
                return actualValue;
            }
            //!
            //! @brief Operator T to return value
            //!
            //! @return T Value of the item
            //!
            operator T()
            {
                return GetValue();
            }
            //!
            //! @brief Get the module output by path
            //!
            //! @param connectorPath Path of the output connector
            //! @return ModuleOut<T>* OutputConnector with path
            //!
            static ModuleOut<T>* GetModuleOutput(std::string connectorPath)
            {
                return GetModule<ModuleOut<T>>(connectorPath, ModuleType::eOutput, GetDataTypeById(typeid(T)));
            }
    };

} // namespace ModelController
