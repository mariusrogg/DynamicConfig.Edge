//!
//! @file BaseOutputConnector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class for output variables
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseConnector.hpp"
#include "BaseModuleIn.hpp"

namespace ModelController
{
    template <typename T>
    class BaseOutputConnector : public BaseModuleIn<T>, public BaseConnector
    {
        protected:
            //!
            //! @brief Set value of the output
            //!
            //! @param value Value to be set to output in percent
            //! @return true Value was successfully set to output
            //! @return false Value could not be set to output
            //!
            virtual bool SetOutputValue(T value) = 0;
            //!
            //! @brief Call SetOutputValue for easier use in derived classes
            //!
            virtual bool SetInputValue(T value) override final
            {
                return SetOutputValue(value);
            }

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
            BaseOutputConnector(std::string name, JsonObject config, BaseModule* parent = nullptr)
                : BaseModuleIn<T>(name, config, parent),
                BaseConnector(name, config, parent)
            { }
            //!
            //! @brief Construct a new Base Output object
            //!
            //! @param name Name of the connector
            //! @param pathConnectedModuleOut Path of the connected module-output
            //! @param parent Parent of the Connector (normally pass this)
            //!
            BaseOutputConnector(std::string name, std::string pathConnectedModuleOut, BaseModule* parent = nullptr)
                : BaseModuleIn<T>(name, pathConnectedModuleOut, parent),
                BaseConnector(name, parent)
            { }
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
                return dynamic_cast<BaseOutputConnector<DT>>(BaseModuleIn<T>::GetModuleInput(connectorPath));
            }
    };
} // namespace ModelController
