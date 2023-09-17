//!
//! @file Connector.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the connector
//! @version 0.0.0
//! @date 2023-09-14
//!
//! @copyright Copyright (c) 2023
//!
#include "Connector.hpp"
#include <sstream>
#include "LittleFS.h"
#include "OnboardPWM.hpp"
#include "WiFiHandler.hpp"

using namespace std;

namespace ModelController
{
    //!
    //! @brief Path to the configuration path
    //!
    std::string Connector::configFilePath = Connector::defaultConfigFile;
    //!
    //! @brief Root connector
    //!
    Connector* Connector::rootConnector = nullptr;
    //!
    //! @brief Construct a new Connector object
    //!
    Connector::Connector(string name, JsonObject config, Connector* parent)
        : parent(parent),
        name(name),
        path((GetParent() != nullptr ? GetParent()->GetPath() : "") + "/" + name)
    {
        Serial.println(GetPath().c_str());
        if (parent)
        {
            parent->children.push_back(this);
        }
        for (JsonPair child : config)
        {
            if (child.value().is<JsonObject>())
            {
                Serial.println(child.key().c_str());
                GenerateConnector(child.key().c_str(), child.value(), this);
            }
        }
    }
    //!
    //! @brief Destruction the Connector object
    //!
    Connector::~Connector()
    {
        if (parent)
        {
            remove(parent->children.begin(), parent->children.end(), this);
        }
    }
    //!
    //! @brief Returns parent of the actual object
    //!
    Connector* Connector::GetParent() const
    {
        return parent;
    }
    //!
    //! @brief Returns path of the actual object
    //!
    string Connector::GetPath() const
    {
        return path;
    }
    //!
    //! @brief Returns name of the actual object
    //!
    string Connector::GetName() const
    {
        return name;
    }
    //!
    //! @brief Generates connector from json
    //!
    Connector* Connector::GenerateConnector(string name, JsonObject connectorConfig, Connector* parent)
    {
        Connector* connector = nullptr;
        if (connectorConfig["type"].is<string>())
        {
            string type = connectorConfig["type"].as<string>();
            if (type == OnboardPWM::type)
            {
                connector = new OnboardPWM(name, connectorConfig, parent);
            }
        }
        return connector;
    }
    //!
    //! @brief Get config of the connector and it's children
    //!
    string Connector::GetConfig()
    {
        stringstream config;
        for (std::vector<Connector*>::iterator it = children.begin(); it != children.end(); it++)
        {
            if (it != children.begin())
            {
                config << ",";
            }
            config << "\"" << (*it)->GetName() << "\": {" << (*it)->GetConfig() << "}";
        }
        return config.str();
    }
    //!
    //! @brief Update config of the controller
    //!
    void Connector::UpdateConfig(JsonObject config)
    {
        if (rootConnector != nullptr)
        {
            delete rootConnector;
            rootConnector = nullptr;
        }

        string hwName = "HW";
        JsonObject cnf(config);


        if (cnf["wifi"]["ssid"].isNull())
        {
            cnf["wifi"]["ssid"] = WiFiHandler::GetSSID();
        }
        if (cnf["wifi"]["password"].isNull())
        {
            cnf["wifi"]["password"] = WiFiHandler::GetPassword();
        }

        WiFiHandler::SetSSIDPassword(cnf["wifi"]["ssid"], cnf["wifi"]["password"]);

        rootConnector = new Connector(hwName, cnf);

        std::string connectors = "{ \"" + rootConnector->GetName() + "\": {" + rootConnector->GetConfig() + "}}";

        // ToDo: Override file on change
        // File file = LittleFS.open(Connector::configFilePath.c_str(), FILE_WRITE);
        // if (!file)
        // {
        //     return;
        // }
        // file.print(connectors.c_str());
        // file.close();
    }
    //!
    //! @brief Initially build config
    //!
    void Connector::InitConfig(std::string configFilePath)
    {
        Connector::configFilePath = configFilePath;
        File file = LittleFS.open(Connector::configFilePath.c_str(), FILE_READ);
        DynamicJsonDocument jsonDoc(32768);

        deserializeJson(jsonDoc, file);

        file.close();
        UpdateConfig(jsonDoc.as<JsonObject>());
    }
} // namespace ModelController
