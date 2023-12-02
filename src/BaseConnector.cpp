//!
//! @file BaseConnector.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the connector
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseConnector.hpp"
#include "OnboardPWM.hpp"
#include "MQTTClient.hpp"
#include "Logger.hpp"

namespace ModelController
{
    //!
    //! @brief Root connector
    //!
    BaseConnector* BaseConnector::rootConnector = nullptr;
    //!
    //! @brief Construct a new Connector object
    //!
    BaseConnector::BaseConnector(std::string name, JsonObject config, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
        : BaseConnector(name, parent, config["shortPath"].is<bool>() ? config["shortPath"].as<bool>() : false, type, dataType)
    {
        Logger::trace("BaseConnector::BaseConnector(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        for (JsonPair child : config)
        {
            if (child.value().is<JsonObject>())
            {
                Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                GenerateConnector(child.key().c_str(), child.value(), this);
            }
        }
    }
    //!
    //! @brief Construct a new Connector object
    //!
    BaseConnector::BaseConnector(std::string name, BaseModule* parent, bool createShortPath, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
    {
        Logger::trace("BaseConnector::BaseConnector(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Initialize(name, parent, createShortPath, type, dataType);
    }
    //!
    //! @brief Destruction of the Base Base Connector object
    //!
    BaseConnector::~BaseConnector()
    {
        Logger::trace("BaseConnector::~BaseConnector() - " + GetPath());
    }
    //!
    //! @brief Generates connector from json
    //!
    BaseConnector* BaseConnector::GenerateConnector(std::string name, JsonObject connectorConfig, BaseConnector* parent)
    {
        Logger::trace("BaseConnector::GenerateConnector(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ")");
        BaseConnector* connector = nullptr;
        if (connectorConfig["type"].is<std::string>())
        {
            std::string type = connectorConfig["type"].as<std::string>();
            if (type == OnboardPWM::type)
            {
                Logger::debug("Generating OnboardPWM");
                connector = new OnboardPWM(name, connectorConfig, parent);
            }
            else if (type == MQTTClient::type)
            {
                Logger::debug("Generating MQTTClient");
                connector = new MQTTClient(name, connectorConfig, parent);
            }
            else
            {
                for (JsonPair child : connectorConfig)
                {
                    if (child.value().is<JsonObject>())
                    {
                        Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                        GenerateConnector(name + "/" + child.key().c_str(), child.value(), parent);
                    }
                }
            }
        }
        return connector;
    }
} // namespace ModelController
