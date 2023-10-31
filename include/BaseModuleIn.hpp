//!
//! @file BaseModuleIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for input values for modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "IBaseModuleIn.hpp"
#include "EventHandling.hpp"
#include "BaseModuleOut.hpp"

namespace ModelController
{
    template <typename T>
    class BaseModuleOut;

    template <typename T>
    class BaseModuleIn : public IBaseModuleIn
    {
        private:
            //!
            //! @brief Actual value of the input
            //!
            T actualValue = 0;
            //!
            //! @brief Path to the connected output module
            //!
            std::string pathConnectedModuleOut;
            //!
            //! @brief Listener called, if value of connected output changed
            //!
            typename Event<T>::Listener* OnOutputChanged = nullptr;
            //!
            //! @brief Listener called, if new output module was created
            //!
            Event<std::string>::Listener* OnModuleOutCreated = nullptr;
            //!
            //! @brief Callback called, if new output was created, to listen to output changed event
            //!
            //! @param pathCreatedOutput Path of the created output
            //!
            void OnOutputCreated(std::string pathCreatedOutput)
            {
                Logger::trace("BaseModuleIn::OnOutputCreated(" + pathCreatedOutput + ") - Module: " + this->GetPath());
                bool outputAvailable = pathCreatedOutput == pathConnectedModuleOut;

                Logger::trace("wildcard: \t" + IBaseModuleOut::wildcardSuffix);
                Logger::trace("pathCreatedOutput: \t" + pathCreatedOutput);
                Logger::trace("pathConnectedModuleOut: \t" + pathConnectedModuleOut);

                size_t wildcardSize = IBaseModuleOut::wildcardSuffix.size();
                size_t pathCreatedOutputSize = pathCreatedOutput.size();
                size_t pathConnectedModuleOutSize = pathConnectedModuleOut.size();

                Logger::trace("wildcardSize: \t" + std::to_string(wildcardSize));
                Logger::trace("pathCreatedOutputSize: \t" + std::to_string(pathCreatedOutputSize));
                Logger::trace("pathConnectedModuleOutSize: \t" + std::to_string(pathConnectedModuleOutSize));

                //! Check if pathCreatedOutput ends with wildcard and pathConnectedModuleOut is child of pathCreatedOutput
                if (!outputAvailable
                    && pathConnectedModuleOutSize >= pathCreatedOutputSize - wildcardSize
                    && pathCreatedOutputSize > wildcardSize
                    && pathCreatedOutput.compare(pathCreatedOutputSize - wildcardSize, wildcardSize, IBaseModuleOut::wildcardSuffix) == 0
                    && pathConnectedModuleOut.compare(0, pathCreatedOutputSize - wildcardSize, pathCreatedOutput.substr(0, pathCreatedOutputSize - wildcardSize)) == 0)
                {
                    outputAvailable = true;
                }

                //! Only set listener, if not set yet
                if (OnOutputChanged == nullptr && outputAvailable)
                {
                    //! Find connected output
                    BaseModuleOut<T>* connectedOutput = BaseModuleOut<T>::GetModuleOutput(pathConnectedModuleOut);
                    if (connectedOutput != nullptr)
                    {
                        //! Listen to ValueChangedEvent of connected output and delete OnModuleOutCreated, that the listener does not get called anymore
                        delete OnModuleOutCreated;
                        OnModuleOutCreated = nullptr;
                        SetOutputChangedEvent(&(connectedOutput->ValueChangedEvent));
                    }
                    //! Create Listener to ModuleOutCreated event, if no matching connectedOutput was found
                    else if (OnModuleOutCreated == nullptr)
                    {
                        OnModuleOutCreated = new Event<std::string>::Listener(&IBaseModuleOut::ModuleOutCreated, [&](std::string path){ this->OnOutputCreated(path); } );
                    }
                }
            }
        protected:
            //!
            //! @brief Set value of the input
            //!
            //! @param value Value to be set to input
            //! @return true Value was successfully set to input
            //! @return false Value could not be set to input
            //!
            virtual bool SetIntputValue(T value) = 0;
        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param config Json config of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleIn(std::string name, JsonObject config, BaseModule* parent = nullptr)
            {
                Initialize(name, parent, ModuleType::eInput, GetDataTypeById(typeid(T)));
                if (config["connectedOut"].is<std::string>())
                {
                    pathConnectedModuleOut = config["connectedOut"].as<std::string>();
                }
                if (!pathConnectedModuleOut.empty())
                {
                    OnOutputCreated(pathConnectedModuleOut);
                }
                ModuleInCreated(this->GetPath());
            }
            //!
            //! @brief Construct a new input module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleOut Path of the connected output
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleIn(std::string name, std::string pathConnectedModuleOut, BaseModule* parent = nullptr)
                : pathConnectedModuleOut(pathConnectedModuleOut)
            {
                Initialize(name, parent, ModuleType::eInput, GetDataTypeById(typeid(T)));
                if (!pathConnectedModuleOut.empty())
                {
                    OnOutputCreated(pathConnectedModuleOut);
                }
                ModuleInCreated(this->GetPath());
            }
            //!
            //! @brief Destruction of the Base Module In object
            //!
            virtual ~BaseModuleIn()
            {
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
                    if (SetIntputValue(value))
                    {
                        actualValue = value;
                        ValueChangedEvent(GetValue());
                    }
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
            //! @return BaseModuleIn<T>* InputConnector with path
            //!
            static BaseModuleIn<T>* GetModuleInput(std::string connectorPath)
            {
                return GetModule<BaseModuleIn<T>>(connectorPath, ModuleType::eInput, GetDataTypeById(typeid(T)));
            }
    };

} // namespace ModelController
