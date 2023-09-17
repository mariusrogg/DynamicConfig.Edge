//!
//! @file BaseInputConnector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class for input variables
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "Connector.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    template <typename T>
    class BaseInputConnector : public Connector
    {
        private:
            //!
            //! @brief Actual value (read from input)
            //!
            T actualValue = 0;

        protected:
            //!
            //! @brief Set the target value for the input
            //!
            //! @param value Target value for input
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
            //! @brief Get the Type of the connector
            //!
            //! @return ConnectorType::input
            //!
            ConnectorType GetType() const
            {
                return ConnectorType::input;
            }

        public:
            //!
            //! @brief Event raised, if value changed
            //!
            Event<T> ValueChangedEvent;
            //!
            //! @brief Construct a new Base Input object
            //!
            //! @param name Name of the connector
            //! @param config Config of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseInputConnector(std::string name, JsonObject config, Connector* parent = nullptr)
                : Connector(name, config, parent)
            { }
            //!
            //! @brief Get the actual value set to input
            //!
            //! @return T Actual value
            //!
            T GetValue() const
            {
                return actualValue;
            }
    };
} // namespace ModelController
