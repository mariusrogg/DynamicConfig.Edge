//!
//! @file ModuleIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for input values for modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "IModuleIn.hpp"
#include "EventHandling.hpp"
#include "ModuleOut.hpp"
#include "Utils.hpp"

namespace ModelController
{
    template <typename T>
    class ModuleOut;

    template <typename T>
    class ModuleIn : public IModuleIn
    {
        private:
            //!
            //! @brief Actual value of the input
            //!
            T actualValue;  // ToDo: initialize type dependent
            //!
            //! @brief Listener called, if value of connected output changed
            //!
            typename Event<T>::Listener* OnOutputChanged = nullptr;
            //!
            //! @brief Listener called, if new output module was created
            //!
            Event<std::string>::Listener* OnModuleOutCreated = nullptr;

        protected:
            //!
            //! @brief Path to the connected output module
            //!
            std::string pathConnectedModuleOut;
            //!
            //! @brief Callback called, if new output was created, to listen to output changed event
            //!
            //! @param pathCreatedOutput Path of the created output
            //!
            void OnOutputCreated(std::string pathCreatedOutput)
            {
                Logger::trace("ModuleIn::OnOutputCreated(" + pathCreatedOutput + ") - Module: " + this->GetPath());
                bool outputAvailable = pathCreatedOutput == pathConnectedModuleOut;

                Logger::trace("wildcard: \t" + IModuleOut::wildcardSuffix);
                Logger::trace("pathCreatedOutput: \t" + pathCreatedOutput);
                Logger::trace("pathConnectedModuleOut: \t" + pathConnectedModuleOut);

                size_t wildcardSize = IModuleOut::wildcardSuffix.size();
                size_t pathCreatedOutputSize = pathCreatedOutput.size();
                size_t pathConnectedModuleOutSize = pathConnectedModuleOut.size();

                Logger::trace("wildcardSize: \t" + std::to_string(wildcardSize));
                Logger::trace("pathCreatedOutputSize: \t" + std::to_string(pathCreatedOutputSize));
                Logger::trace("pathConnectedModuleOutSize: \t" + std::to_string(pathConnectedModuleOutSize));

                //! Check if pathCreatedOutput ends with wildcard and pathConnectedModuleOut is child of pathCreatedOutput
                if (!outputAvailable
                    && pathConnectedModuleOutSize >= pathCreatedOutputSize - wildcardSize
                    && pathCreatedOutputSize > wildcardSize
                    && pathCreatedOutput.compare(pathCreatedOutputSize - wildcardSize, wildcardSize, IModuleOut::wildcardSuffix) == 0
                    && pathConnectedModuleOut.compare(0, pathCreatedOutputSize - wildcardSize, pathCreatedOutput.substr(0, pathCreatedOutputSize - wildcardSize)) == 0)
                {
                    outputAvailable = true;
                }

                //! Only set listener, if not set yet
                if (OnOutputChanged == nullptr && outputAvailable)
                {
                    Logger::trace("Try to find connected output with path: " + pathConnectedModuleOut);
                    //! Find connected output
                    ModuleOut<T>* connectedOutput = ModuleOut<T>::GetModuleOutput(pathConnectedModuleOut);
                    if (connectedOutput != nullptr)
                    {
                        Logger::trace("Connected output found");
                        //! Listen to ValueChangedEvent of connected output and delete OnModuleOutCreated, that the listener does not get called anymore
                        delete OnModuleOutCreated;
                        OnModuleOutCreated = nullptr;
                        SetOutputChangedEvent(&(connectedOutput->ValueChangedEvent));
                    }
                    //! Create Listener to ModuleOutCreated event, if no matching connectedOutput was found
                    else if (OnModuleOutCreated == nullptr)
                    {
                        OnModuleOutCreated = new Event<std::string>::Listener(&IModuleOut::ModuleOutCreated, [&](std::string path){ this->OnOutputCreated(path); } );
                    }
                }
            }
            //!
            //! @brief Get the Path Connected Module Out object out of the config
            //!
            //! @param name Name of the actual object (ModuleIn)
            //! @param parentConfig Json config of the parent object or json string with path to connected output module
            //! @return std::string Extracted path of the connected output module, empty string if not found in parentConfig
            //!
            std::string GetPathConnectedModuleOut(std::string name, JsonVariant parentConfig)
            {
                std::string path;
                Logger::trace("ModuleIn::GetPathConnectedModuleOut(" + name + ", " + parentConfig.as<std::string>() + ")");
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
            //!
            //! @brief Listener called, if input value changed
            //!
            typename Event<T>::Listener* inputChanged = nullptr;

        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleOut Path of the connected output
            //! @param inputChanged Function called, if input changed
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleIn(std::string name, std::string pathConnectedModuleOut, std::function<void(T)> onInputChanged, BaseModule* parent = nullptr)
                : IModuleIn(name, parent, GetDataTypeById(typeid(T))),
                pathConnectedModuleOut(pathConnectedModuleOut)
            {
                this->inputChanged = new typename Event<T>::Listener(&(this->ValueChangedEvent), onInputChanged);
                Logger::trace("ModuleIn::ModuleIn(" + name + ", " + this->pathConnectedModuleOut + ")");
                if (!this->pathConnectedModuleOut.empty() && this->pathConnectedModuleOut != "none")
                {
                    OnOutputCreated(this->pathConnectedModuleOut);
                }
                ModuleInCreated(this->GetPath());
                if (this->pathConnectedModuleOut.empty() && !Utils::StartsWith(GetPath(), apiPath))
                {
                    this->pathConnectedModuleOut = apiPath + GetShortPath();
                    Logger::trace("Creating connection to default connector for " + this->GetPath());
                    this->OnOutputCreated(this->pathConnectedModuleOut);
                }
            }
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param inputChanged Function called, if input changed
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleIn(std::string name, std::function<void(std::string)> onInputChanged, BaseModule* parent = nullptr)
                : ModuleIn<T>(name, "none", [=](T value){ onInputChanged(Utils::ToString(value)); }, parent)
            {
            }
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param parentConfig Json config of the parent object or json string with path to connected output module
            //! @param inputChanged Function called, if input changed
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleIn(std::string name, JsonVariant parentConfig, std::function<void(T)> onInputChanged, BaseModule* parent = nullptr)
                : ModuleIn<T>(name, GetPathConnectedModuleOut(name, parentConfig), onInputChanged, parent)
            {
            }
            //!
            //! @brief Destruction of the Base Module In object
            //!
            virtual ~ModuleIn()
            {
                delete inputChanged;
                delete OnOutputChanged;
                delete OnModuleOutCreated;
            }
            //!
            //! @brief Set the Listener for OutputChagnedEvent
            //!
            //! @param event Event to listen to
            //!
            //! @return true Event was set successfully
            //! @return false Event could not be set
            //!
            bool SetOutputChangedEvent(Event<T>* event)
            {
                bool retVal = false;
                //! Prevent, that OnOuputChanged is set by output, while object is waiting for creation of connected output
                if (OnOutputChanged == nullptr && OnModuleOutCreated == nullptr)
                {
                    OnOutputChanged = new typename Event<T>::Listener(event, [&](T value){ this->SetValue(value); } );
                    retVal = true;
                    Logger::trace("OnOutputChanged set");
                }
                return retVal;
            }
            //!
            //! @brief Set the target value for the output
            //!
            //! @param value Target value for output
            //!
            void SetValue(T value)
            {
                if (actualValue != value)
                {
                    actualValue = value;
                    ValueChangedEvent(GetValue());
                }
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
            //! @brief Get the module input by path
            //!
            //! @param connectorPath Path of the input connector
            //! @return ModuleIn<T>* InputConnector with path
            //!
            static ModuleIn<T>* GetModuleInput(std::string connectorPath)
            {
                return GetModule<ModuleIn<T>>(connectorPath, ModuleType::eInput, GetDataTypeById(typeid(T)));
            }
    };

} // namespace ModelController
