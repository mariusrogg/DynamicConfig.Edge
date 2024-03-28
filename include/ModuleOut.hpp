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
        protected:
            //!
            //! @brief Path to the connected output module
            //!
            std::string pathConnectedModuleIn;
        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Variable is connected to API, if path doesn't starts with BaseModule::apiPath
            //!
            //! @return true If connected to API
            //! @return false If connected to output
            //!
            virtual bool IsAPIConnected() const override
            {
                return !Utils::StartsWith(GetPath(), apiPath);
            }
            //!
            //! @brief Construct a new output module
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleIn Path of the connected input
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ModuleOut(std::string name, BaseModule* parent = nullptr)
                : IModuleOut(name, parent, GetDataTypeById(typeid(T)))
            {
                Logger::trace("Raising ModuleOutCreated(" + this->GetPath() + ")");
                ModuleOutCreated(this->GetPath());
                if (!Utils::StartsWith(GetPath(), apiPath))
                {
                    Logger::trace("Creating connection to default connector for " + this->GetPath());
                    // Create and set event to write changes to API variable
                    ModuleIn<T>* connectedAPIVariable = ModuleIn<T>::GetModuleInput(apiPath + this->GetPath());
                    if (connectedAPIVariable != nullptr)
                    {
                        connectedAPIVariable->SetOutputChangedEvent(&(this->ValueChangedEvent));
                    }
                }
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
