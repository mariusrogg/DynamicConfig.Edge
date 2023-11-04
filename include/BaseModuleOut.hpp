//!
//! @file BaseModuleOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for output values for modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"
#include "IBaseModuleOut.hpp"
#include "BaseModuleIn.hpp"

namespace ModelController
{
    template <typename T>
    class BaseModuleIn;

    template <typename T>
    class BaseModuleOut : public IBaseModuleOut
    {
        private:
            //!
            //! @brief Actual value of the output
            //!
            T actualValue = 0;
            //!
            //! @brief Listener called, if new output module was created
            //!
            Event<std::string>::Listener* OnModuleInCreated = nullptr;
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
                Logger::trace("BaseModuleOut::OnInputCreated(" + pathCreatedInput + ") - Module: " + this->GetPath());
                bool inputAvailable = pathCreatedInput == pathConnectedModuleIn;

                Logger::trace("wildcard: \t" + IBaseModuleIn::wildcardSuffix);
                Logger::trace("pathCreatedInput: \t" + pathCreatedInput);
                Logger::trace("pathConnectedModuleIn: \t" + pathConnectedModuleIn);

                size_t wildcardSize = IBaseModuleIn::wildcardSuffix.size();
                size_t pathCreatedInputSize = pathCreatedInput.size();
                size_t pathConnectedModuleInSize = pathConnectedModuleIn.size();

                Logger::trace("wildcardSize: \t" + std::to_string(wildcardSize));
                Logger::trace("pathCreatedInputSize: \t" + std::to_string(pathCreatedInputSize));
                Logger::trace("pathConnectedModuleInSize: \t" + std::to_string(pathConnectedModuleInSize));

                //! Check if pathCreatedInput ends with wildcard and pathConnectedModuleIn is child of pathCreatedInput
                if (!inputAvailable
                    && pathConnectedModuleInSize >= pathCreatedInputSize - wildcardSize
                    && pathCreatedInputSize > wildcardSize
                    && pathCreatedInput.compare(pathCreatedInputSize - wildcardSize, wildcardSize, IBaseModuleIn::wildcardSuffix) == 0
                    && pathConnectedModuleIn.compare(0, pathCreatedInputSize - wildcardSize, pathCreatedInput.substr(0, pathCreatedInputSize - wildcardSize)) == 0)
                {
                    inputAvailable = true;
                }

                //! Only set listener, if not set yet
                if (inputAvailable)
                {
                    //! Find connected input
                    BaseModuleIn<T>* connectedInput = BaseModuleIn<T>::GetModuleInput(pathConnectedModuleIn);
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
                        OnModuleInCreated = new Event<std::string>::Listener(&IBaseModuleIn::ModuleInCreated, [&](std::string path){ this->OnInputCreated(path); } );
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
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleOut(std::string name, BaseModule* parent = nullptr)
            {
                Initialize(name, parent, ModuleType::eOutput, GetDataTypeById(typeid(T)));
                Logger::trace("Raising ModuleOutCreated(" + this->GetPath() + ")");
                ModuleOutCreated(this->GetPath());
            }
            //!
            //! @brief Construct a new output module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleIn Path of the connected input
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleOut(std::string name, std::string pathConnectedModuleIn, BaseModule* parent = nullptr)
                : pathConnectedModuleIn(pathConnectedModuleIn)
            {
                Initialize(name, parent, ModuleType::eOutput, GetDataTypeById(typeid(T)));
                if (!pathConnectedModuleIn.empty())
                {
                    OnInputCreated(pathConnectedModuleIn);
                }
                Logger::trace("Raising ModuleOutCreated(" + this->GetPath() + ")");
                ModuleOutCreated(this->GetPath());
            }
            //!
            //! @brief Construct a new output module
            //!
            //! @param name Name of the connector
            //! @param config Json config of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleOut(std::string name, JsonVariant config, BaseModule* parent = nullptr)
            {
                Initialize(name, parent, ModuleType::eOutput, GetDataTypeById(typeid(T)));
                if (config.is<JsonObject>() && config["connectedIn"].is<std::string>())
                {
                    pathConnectedModuleIn = config["connectedIn"].as<std::string>();
                }
                else if (config.is<std::string>())
                {
                    pathConnectedModuleIn = config.as<std::string>();
                }
                if (!pathConnectedModuleIn.empty())
                {
                    OnInputCreated(pathConnectedModuleIn);
                }
                Logger::trace("Raising ModuleOutCreated(" + this->GetPath() + ")");
                ModuleOutCreated(this->GetPath());
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
            //! @brief Get the module output by path
            //!
            //! @param connectorPath Path of the output connector
            //! @return BaseModuleOut<T>* OutputConnector with path
            //!
            static BaseModuleOut<T>* GetModuleOutput(std::string connectorPath)
            {
                return GetModule<BaseModuleOut<T>>(connectorPath, ModuleType::eOutput, GetDataTypeById(typeid(T)));
            }
    };

} // namespace ModelController
