//!
//! @file Connector.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of Hardware connectors
//! @version 0.0.0
//! @date 2022-12-10
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
    private:
        //!
        //! @brief List with all connectors in programm and their paths
        //!
        static std::map<std::string, Connector*> connectors;
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
        static T* GetConnector(std::string connectorPath)
        {
            Connector* connector = nullptr;
            if (connectors.find(connectorPath) != connectors.end())
            {
                connector = connectors[connectorPath];
            }
            return dynamic_cast<T*>(connector);
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