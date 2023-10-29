//!
//! @file BaseConnector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of Hardware connectors
//!
//! @copyright Copyright (c) 2022
//!
//!
#pragma once

#include <ArduinoJson.h>
#include <string>
#include "BaseModule.hpp"
#include "Logger.hpp"

namespace ModelController
{
    class BaseConnector : public virtual BaseModule
    {
        public:
            //!
            //! @brief Root connector of the hardware configuration
            //!
            static BaseConnector* rootConnector;
            //!
            //! @brief Construct a new Connector object
            //!
            //! @param name Name of the connector
            //! @param config Config of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //! @param type Type of the connector
            //! @param dataType DataType of the connector
            //!
            BaseConnector(std::string name, JsonObject config, BaseModule* parent = nullptr, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Construct a new Connector object
            //!
            //! @param name Name of the connector
            //! @param parent Parent of the Connector (normally pass this)
            //! @param type Type of the connector
            //! @param dataType DataType of the connector
            //!
            BaseConnector(std::string name, BaseModule* parent = nullptr, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Destroy the Connector object
            //!
            virtual ~BaseConnector();
            //!
            //! @brief Generate connector from json
            //!
            //! @param connectorConfig Config of the connector
            //! @return BaseConnector* Created connector
            //!
            static BaseConnector* GenerateConnector(std::string name, JsonObject connectorConfig, BaseConnector* parent = nullptr);
            //!
            //! @brief Get a connector by path
            //!
            //! @tparam T Class type of the connector
            //! @param type Type of the connector
            //! @param dataType Datatype of the connector
            //! @param connectorPath Path of the connector
            //! @return T* BaseConnector with path
            //!
            template<class T>
            T* GetChildConnector(std::string connectorPath, BaseModule::ModuleType type = ModuleType::eUndefined, BaseModule::ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                Logger::trace("BaseConnector::GetChildConnector(" + connectorPath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
                return dynamic_cast<T*>(GetChild(connectorPath, type, dataType));
            }
            //!
            //! @brief Get a connector by path
            //!
            //! @tparam T Class type of the connector
            //! @param type Type of the connector
            //! @param dataType Datatype of the connector
            //! @param connectorPath Path of the connector
            //! @return T* BaseConnector with path
            //!
            template<class T>
            static T* GetConnector(std::string connectorPath, BaseModule::ModuleType type = ModuleType::eUndefined, BaseModule::ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                Logger::trace("BaseConnector::GetConnector(" + connectorPath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
                return rootConnector == nullptr ? nullptr : rootConnector->GetChildConnector<T>(connectorPath, type, dataType);
            }
    };
} // namespace ModelController