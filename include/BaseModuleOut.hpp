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

namespace ModelController
{
    template <typename T>
    class BaseModuleOut : virtual BaseModule
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
            //! @tparam DT Underlying (primitive) type of the output connector
            //! @param connectorPath Path of the output connector
            //! @return BaseModuleOut<DT> OutputConnector with path
            //!
            template<class DT>
            static BaseModuleOut<DT> GetModuleOutput(std::string connectorPath)
            {
                return GetModule<BaseModuleOut<DT>>(connectorPath, ModuleType::eOutput, GetDataTypeById(typeid(DT)));
            }
    };

} // namespace ModelController
