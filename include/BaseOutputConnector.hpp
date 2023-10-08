//!
//! @file BaseOutputConnector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class for output variables
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "Connector.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    template <typename T>
    class BaseOutputConnector : public Connector
    {
        private:
            //!
            //! @brief Actual value (set to output)
            //!
            T actualValue = 0;

        protected:
            //!
            //! @brief Set value of the output
            //!
            //! @param value Value to be set to output in percent
            //! @return true Value was successfully set to output
            //! @return false Value could not be set to output
            //!
            virtual bool SetOutputValue(T value) = 0;

        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Construct a new Base Output object
            //!
            //! @param name Name of the connector
            //! @param config Config of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseOutputConnector(std::string name, JsonObject config, Connector* parent = nullptr)
                : Connector(name, config, parent, ConnectorType::eOutput, GetDataTypeById(typeid(T)))
            { }
            //!
            //! @brief Construct a new Base Output object
            //!
            //! @param name Name of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseOutputConnector(std::string name, Connector* parent = nullptr)
                : Connector(name, parent, ConnectorType::eOutput, GetDataTypeById(typeid(T)))
            { }
            //!
            //! @brief Set the target value for the output
            //!
            //! @param value Target value for output
            //!
            void SetValue(T value)
            {
                if (actualValue != value)
                {
                    if (SetOutputValue(value))
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
            //! @brief Get the output connector by path
            //!
            //! @tparam T Underlying (primitive) type of the output connector
            //! @param connectorPath Path of the output connector
            //! @return BaseOutputConnector<T> OutputConnector with path
            //!
            template<class DT>
            static BaseOutputConnector<DT> GetOutputConnector(std::string connectorPath)
            {
                return GetConnector<BaseOutputConnector<DT>>(connectorPath, ConnectorType::eOutput, GetDataTypeById(typeid(T)));
            }
    };
} // namespace ModelController
