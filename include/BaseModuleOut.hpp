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

namespace ModelController
{
    template <typename T>
    class BaseModuleOut : public IBaseModuleOut
    {
        private:
            //!
            //! @brief Actual value of the output
            //!
            T actualValue = 0;
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
