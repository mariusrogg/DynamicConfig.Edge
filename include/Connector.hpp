//!
//! @file Connector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of Hardware connectors
//!
//! @copyright Copyright (c) 2022
//!
//!
#pragma once

#include <ArduinoJson.h>
#include <string>
#include <vector>
#include <map>

namespace ModelController
{
    class Connector
    {
    public:
        //!
        //! @brief Type of the connector
        //!
        enum class ConnectorType
        {
            eUndefined,      //!< Type is not defined
            eInput,          //!< Type is input
            eOutput,         //!< Type is output
            eNone,           //!< Type is neither input nor output
        };
        //!
        //! @brief Get ConnectorType as string
        //!
        //! @param type ConnectorType to get
        //! @return std::string Name of the type
        //!
        static std::string TypeToString(ConnectorType type);
        //!
        //! @brief Data type of the connected variable
        //!
        enum class ConnectorDataType
        {
            eUndefined, //!< Type is not defined
            eNone,      //!< Connector has no type
            eDouble,    //!< Connected data type is double
            eFloat,     //!< Connected data type is float
            eString,    //!< Connected data type is std::string
            eInt8,      //!< Connected data type is signed 8 bit integer
            eInt16,     //!< Connected data type is signed 16 bit integer
            eInt32,     //!< Connected data type is signed 32 bit integer
            eInt64,     //!< Connected data type is signed 64 bit integer
            eUInt8,     //!< Connected data type is unsigned 8 bit integer
            eUInt16,    //!< Connected data type is unsigned 16 bit integer
            eUInt32,    //!< Connected data type is unsigned 32 bit integer
            eUInt64,    //!< Connected data type is unsigned 64 bit integer
            eBool,      //!< Connected data type is boolean
        };
        //!
        //! @brief Get ConnectorDataType as string
        //!
        //! @param dataType ConnectorDataType to get
        //! @return std::string Name of the dataType
        //!
        static std::string DataTypeToString(ConnectorDataType dataType);
        //!
        //! @brief Get the Type of the connector
        //!
        //! @return ConnectorType Type of the object
        //!
        ConnectorType GetType() const;
        //!
        //! @brief Get the DataType of the connector
        //!
        //! @return ConnectorDataType DataType of the object
        //!
        ConnectorDataType GetDataType() const;

    private:
        //!
        //! @brief Root connector of the hardware configuration
        //!
        static Connector* rootConnector;
        //!
        //! @brief Parent of the connector object
        //!
        Connector* parent;
        //!
        //! @brief Children of the connector object
        //!
        std::vector<Connector*> children;
        //!
        //! @brief Path of the connector object
        //!
        std::string path;
        //!
        //! @brief Name of the connector object
        //!
        std::string name;
        //!
        //! @brief Default config file path
        //!
        static std::string configFilePath;

    protected:
        //!
        //! @brief Type of the connector
        //!
        ConnectorType connectorType = ConnectorType::eUndefined;
        //!
        //! @brief Data-Type of the connector
        //!
        ConnectorDataType connectorDataType = ConnectorDataType::eUndefined;
        //!
        //! @brief Get child of the object
        //!
        //! @param connectorPath Path of the child
        //! @param type ConnectorType of the child
        //! @param dataType ConnectorDataType of the child
        //! @return Connector* Child, nullptr if not found
        //!
        virtual Connector* GetChild(std::string connectorPath, ConnectorType type = ConnectorType::eUndefined, ConnectorDataType dataType = ConnectorDataType::eUndefined);

    public:
        //!
        //! @brief Get the data type by typeid
        //!
        //! @param typeInfo Usually pass typeid(object) or typeid(Type)
        //! @return ConnectorDataType Matching data type
        //!
        static ConnectorDataType GetDataTypeById(const std::type_info& typeInfo);
        //!
        //! @brief Default config file path
        //!
        static constexpr const char* defaultConfigFile = "/Connectors.json";
        //!
        //! @brief Construct a new Connector object
        //!
        //! @param name Name of the connector
        //! @param config Config of the connector
        //! @param parent Parent of the Connector (normally pass this)
        //! @param ConnectorType Type of the connector
        //! @param connectorDataType DataType of the connector
        //!
        Connector(std::string name, JsonObject config, Connector* parent = nullptr, ConnectorType ConnectorType = ConnectorType::eUndefined, ConnectorDataType connectorDataType = ConnectorDataType::eUndefined);
        //!
        //! @brief Construct a new Connector object
        //!
        //! @param name Name of the connector
        //! @param parent Parent of the Connector (normally pass this)
        //! @param ConnectorType Type of the connector
        //! @param connectorDataType DataType of the connector
        //!
        Connector(std::string name, Connector* parent = nullptr, ConnectorType ConnectorType = ConnectorType::eUndefined, ConnectorDataType connectorDataType = ConnectorDataType::eUndefined);
        //!
        //! @brief Destroy the Connector object
        //!
        virtual ~Connector();
        //!
        //! @brief Get the Parent of the object
        //!
        //! @return Connector* Parent of the object
        //!
        Connector* GetParent() const;
        //!
        //! @brief Get the path of the object
        //!
        //! @return std::string Path of the object
        //!
        std::string GetPath() const;
        //!
        //! @brief Get the name of the object
        //!
        //! @return std::string Name of the object
        //!
        std::string GetName() const;
        //!
        //! @brief Generate connector from json
        //!
        //! @param connectorConfig Config of the connector
        //! @return Connector* Created connector
        //!
        static Connector* GenerateConnector(std::string name, JsonObject connectorConfig, Connector* parent = nullptr);
        //!
        //! @brief Get the config of the connector
        //!
        //! @return string Config created
        //!
        virtual std::string GetConfig();
        //!
        //! @brief Get a connector by path
        //!
        //! @param connectorPath Path of the connector
        //! @return T* Connector with path
        //!
        template<class T>
        T* GetChildConnector(std::string connectorPath, ConnectorType connectorType = ConnectorType::eUndefined, ConnectorDataType dataType = ConnectorDataType::eUndefined)
        {
            return dynamic_cast<T*>(GetChild(connectorPath, connectorType, dataType));
        }
        //!
        //! @brief Get a connector by path
        //!
        //! @param connectorPath Path of the connector
        //! @return T* Connector with path
        //!
        template<class T>
        static T* GetConnector(std::string connectorPath, ConnectorType connectorType = ConnectorType::eUndefined, ConnectorDataType dataType = ConnectorDataType::eUndefined)
        {
            return rootConnector == nullptr ? nullptr : rootConnector->GetChildConnector<T>(connectorPath, connectorType, dataType);
        }
        //!
        //! @brief Update hardware configuration
        //!
        //! @param config Json object with new hardware config
        //!
        static void UpdateConfig(JsonObject config);
        //!
        //! @brief Initialize hardware configuration
        //!
        static void InitConfig(std::string configFilePath = defaultConfigFile);
    };
} // namespace ModelController