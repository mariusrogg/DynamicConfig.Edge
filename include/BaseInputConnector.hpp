//!
//! @file BaseInputConnector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class for input variables
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseConnector.hpp"
#include "BaseModuleOut.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    template <typename T>
    class BaseInputConnector : public BaseModuleOut<T>, public BaseConnector
    {
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
            BaseInputConnector(std::string name, JsonObject config, BaseModule* parent = nullptr)
                : BaseModuleOut<T>(name, parent),
                BaseConnector(name, config, parent)
            { }
            //!
            //! @brief Construct a new Base Input Connector object
            //!
            //! @param name Name of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseInputConnector(std::string name, BaseModule* parent = nullptr)
                : BaseModuleOut<T>(name, parent),
                BaseConnector(name, parent)
            { }
            //!
            //! @brief Get the input connector by path
            //!
            //! @tparam T Underlying (primitive) type of the input connector
            //! @param connectorPath Path of the input connector
            //! @return BaseInputConnector<T> InputConnector with path
            //!
            template<class DT>
            static BaseInputConnector<DT> GetInputConnector(std::string connectorPath)
            {
                return dynamic_cast<BaseInputConnector<DT>>(BaseModuleOut<T>::GetModuleOutput(connectorPath));
            }
    };
} // namespace ModelController
