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
            input,          //!< Type is input
            output,         //!< Type is output
            none,           //!< Type is neither input nor output
            undefined,      //!< Type is not defined
        };
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
        //! @brief Get the Type of the connector
        //!
        //! @return undefined if not overridden by derived class
        //!
        virtual ConnectorType GetType() const
        {
            return ConnectorType::undefined;
        }

    public:
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
        //!
        Connector(std::string name, JsonObject config, Connector* parent = nullptr);
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
        T* GetChildConnector(std::string connectorPath, ConnectorType type = ConnectorType::undefined)
        {
            // Trim '/' at start of path
            connectorPath = connectorPath.substr(connectorPath.find_first_not_of('/'));
            Connector* connector = nullptr;
            std::string name = connectorPath;
            std::string subPath = "";
            // Split path into name and further path
            if (connectorPath.find('/') != std::string::npos)
            {
                name = connectorPath.substr(0, connectorPath.find('/'));
                subPath = connectorPath.substr(connectorPath.find('/') + 1);
            }
            for (Connector* child : children)
            {
                if (child->GetName() == name)
                {
                    if (subPath.size() == 0)
                    {
                        if (type == ConnectorType::undefined || connector->GetType() == type)
                        {
                            connector = child;
                        }
                    }
                    else
                    {
                        connector = child->GetChildConnector<T>(subPath, type);
                    }
                    break;
                }
            }
            return dynamic_cast<T*>(connector);
        }
        //!
        //! @brief Get a connector by path
        //!
        //! @param connectorPath Path of the connector
        //! @return T* Connector with path
        //!
        template<class T>
        static T* GetConnector(std::string connectorPath, ConnectorType type = ConnectorType::undefined)
        {
            return rootConnector == nullptr ? nullptr : rootConnector->GetChildConnector<T>(connectorPath, type);
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