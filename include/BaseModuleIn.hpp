//!
//! @file BaseModuleIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for input values for modules
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    template <typename T>
    class BaseModuleIn : virtual BaseModule
    {
        private:
            //!
            //! @brief Actual value of the input
            //!
            T actualValue = 0;
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
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseModuleIn(std::string name, BaseModule* parent = nullptr)
            {
                Initialize(name, parent, ModuleType::eInput, GetDataTypeById(typeid(T)));
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
            //! @tparam DT Underlying (primitive) type of the input connector
            //! @param connectorPath Path of the input connector
            //! @return BaseModuleIn<DT>* InputConnector with path
            //!
            template<class DT>
            static BaseModuleIn<DT>* GetModuleInput(std::string connectorPath)
            {
                return GetModule<BaseModuleIn<DT>>(connectorPath, ModuleType::eInput, GetDataTypeById(typeid(DT)));
            }
    };

} // namespace ModelController
