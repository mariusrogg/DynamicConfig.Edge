//!
//! @file BaseConnector.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the connector
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseConnector.hpp"
#include <sstream>
#include "LittleFS.h"
#include "OnboardPWM.hpp"
#include "WiFiHandler.hpp"
#include "MQTTClient.hpp"

using namespace std;

namespace ModelController
{
    //!
    //! @brief Switch through possible types and return type as string
    //!
    std::string BaseConnector::TypeToString(ConnectorType type)
    {
        std::string name;
        switch (type)
        {
            case ConnectorType::eUndefined:
                name = "Undefined";
                break;
            case ConnectorType::eInput:
                name = "Input";
                break;
            case ConnectorType::eOutput:
                name = "Output";
                break;
            case ConnectorType::eNone:
                name = "None";
                break;
            default:
                break;
        }
        return name;
    }
    //!
    //! @brief Switch through possible datatypes and return data type as string
    //!
    std::string BaseConnector::DataTypeToString(ConnectorDataType dataType)
    {
        std::string name;
        switch (dataType)
        {
            case ConnectorDataType::eUndefined:
                name = "Undefined";
                break;
            case ConnectorDataType::eNone:
                name = "None";
                break;
            case ConnectorDataType::eDouble:
                name = "Double";
                break;
            case ConnectorDataType::eFloat:
                name = "Float";
                break;
            case ConnectorDataType::eString:
                name = "String";
                break;
            case ConnectorDataType::eInt8:
                name = "Int8";
                break;
            case ConnectorDataType::eInt16:
                name = "Int16";
                break;
            case ConnectorDataType::eInt32:
                name = "Int32";
                break;
            case ConnectorDataType::eInt64:
                name = "Int64";
                break;
            case ConnectorDataType::eUInt8:
                name = "UInt8";
                break;
            case ConnectorDataType::eUInt16:
                name = "UInt16";
                break;
            case ConnectorDataType::eUInt32:
                name = "UInt32";
                break;
            case ConnectorDataType::eUInt64:
                name = "UInt64";
                break;
            case ConnectorDataType::eBool:
                name = "Bool";
                break;
            default:
                break;
        }
        return name;
    }
    //!
    //! @brief Returns connectorType
    //!
    BaseConnector::ConnectorType BaseConnector::GetType() const
    {
        return connectorType;
    }
    //!
    //! @brief Returns connectorDataType
    //!
    BaseConnector::ConnectorDataType BaseConnector::GetDataType() const
    {
        return connectorDataType;
    }
    //!
    //! @brief Returns child of the connector
    //!
    BaseConnector* BaseConnector::GetChild(std::string connectorPath, ConnectorType type, ConnectorDataType dataType)
    {
        //! @brief Trim '/' at start of path
        connectorPath = connectorPath.substr(connectorPath.find_first_not_of('/'));
        BaseConnector* connector = nullptr;
        std::string subPath = "";
        //! @brief Iterate through children
        for (BaseConnector* child : children)
        {
            std::string name = child->GetName();
            size_t posName = connectorPath.find(name);
            size_t sizeName = name.size();
            size_t sizePath = connectorPath.size();

            //! @brief Check if path starts with name of child
            if (sizePath >= sizeName && posName == 0)
            {
                size_t posSlash = connectorPath.find_first_of('/', child->GetName().size());
                //! @brief Check if name is adressed by path (name eqal path or path starts with name + '/')
                if (sizeName == sizePath || posSlash == sizeName)
                {
                    //! @brief Check if size of path is longer than name -> searched element is grandchild
                    if (sizePath >= sizeName + 1)
                    {
                        subPath = connectorPath.substr(child->GetName().size() + 1);
                        connectorPath = connectorPath.substr(connectorPath.find_first_not_of("/"));
                        connector = child->GetChild(subPath, type, dataType);
                    }
                    //! @brief Path is equal to name -> searched element is child
                    else
                    {
                        //! @brief Check if type and data type of child are equal to found childs type and data type
                        if ((type == ConnectorType::eUndefined || child->GetType() == type)
                            && (dataType == ConnectorDataType::eUndefined || dataType == child-> GetDataType()))
                        {
                            connector = child;
                        }
                    }
                    //! @brief Break loop if searched connector is found
                    if (connector != nullptr)
                    {
                        break;
                    }
                }
            }
        }
        return connector;
    }
    //!
    //! @brief Check possible type infos and return corresponding data type
    //!
    BaseConnector::ConnectorDataType BaseConnector::GetDataTypeById(const std::type_info& typeInfo)
    {
        ConnectorDataType type = ConnectorDataType::eUndefined;
        if (typeInfo.hash_code() == typeid(double).hash_code())
        {
            type = ConnectorDataType::eDouble;
        }
        else if (typeInfo.hash_code() == typeid(float).hash_code())
        {
            type = ConnectorDataType::eFloat;
        }
        else if (typeInfo.hash_code() == typeid(std::string).hash_code())
        {
            type = ConnectorDataType::eString;
        }
        else if (typeInfo.hash_code() == typeid(int8_t).hash_code())
        {
            type = ConnectorDataType::eInt8;
        }
        else if (typeInfo.hash_code() == typeid(int16_t).hash_code())
        {
            type = ConnectorDataType::eInt16;
        }
        else if (typeInfo.hash_code() == typeid(int32_t).hash_code())
        {
            type = ConnectorDataType::eInt32;
        }
        else if (typeInfo.hash_code() == typeid(int64_t).hash_code())
        {
            type = ConnectorDataType::eInt64;
        }
        else if (typeInfo.hash_code() == typeid(uint8_t).hash_code())
        {
            type = ConnectorDataType::eUInt8;
        }
        else if (typeInfo.hash_code() == typeid(uint16_t).hash_code())
        {
            type = ConnectorDataType::eUInt16;
        }
        else if (typeInfo.hash_code() == typeid(uint32_t).hash_code())
        {
            type = ConnectorDataType::eUInt32;
        }
        else if (typeInfo.hash_code() == typeid(uint64_t).hash_code())
        {
            type = ConnectorDataType::eUInt64;
        }
        else if (typeInfo.hash_code() == typeid(boolean).hash_code() || typeInfo.hash_code() == typeid(bool).hash_code())
        {
            type = ConnectorDataType::eBool;
        }
        return type;
    }
    //!
    //! @brief Path to the configuration path
    //!
    std::string BaseConnector::configFilePath = BaseConnector::defaultConfigFile;
    //!
    //! @brief Root connector
    //!
    BaseConnector* BaseConnector::rootConnector = nullptr;
    //!
    //! @brief Construct a new Connector object
    //!
    BaseConnector::BaseConnector(string name, JsonObject config, BaseConnector* parent, ConnectorType connectorType, ConnectorDataType dataType)
        : BaseConnector(name, parent, connectorType, dataType)
    {
        for (JsonPair child : config)
        {
            if (child.value().is<JsonObject>())
            {
                Serial.println(child.key().c_str());
                GenerateConnector(child.key().c_str(), child.value(), this);
            }
        }
    }
    BaseConnector::BaseConnector(string name, BaseConnector* parent, ConnectorType connectorType, ConnectorDataType dataType)
        : parent(parent),
        connectorType(connectorType),
        connectorDataType(dataType)
    {
        this->name = name.substr(name.find_first_not_of("/"));
        this->path = (GetParent() != nullptr ? GetParent()->GetPath() : "") + "/" + this->name;
        if (parent)
        {
            parent->children.push_back(this);
        }
    }
    //!
    //! @brief Destruction the Connector object
    //!
    BaseConnector::~BaseConnector()
    {
        if (parent)
        {
            remove(parent->children.begin(), parent->children.end(), this);
        }
    }
    //!
    //! @brief Returns parent of the actual object
    //!
    BaseConnector* BaseConnector::GetParent() const
    {
        return parent;
    }
    //!
    //! @brief Returns path of the actual object
    //!
    string BaseConnector::GetPath() const
    {
        return path;
    }
    //!
    //! @brief Returns name of the actual object
    //!
    string BaseConnector::GetName() const
    {
        return name;
    }
    //!
    //! @brief Generates connector from json
    //!
    BaseConnector* BaseConnector::GenerateConnector(string name, JsonObject connectorConfig, BaseConnector* parent)
    {
        BaseConnector* connector = nullptr;
        if (connectorConfig["type"].is<string>())
        {
            string type = connectorConfig["type"].as<string>();
            if (type == OnboardPWM::type)
            {
                connector = new OnboardPWM(name, connectorConfig, parent);
            }
            else if (type == MQTTClient::type)
            {
                connector = new MQTTClient(name, connectorConfig, parent);
            }
        }
        return connector;
    }
    //!
    //! @brief Get config of the connector and it's children
    //!
    string BaseConnector::GetConfig()
    {
        stringstream config;
        for (std::vector<BaseConnector*>::iterator it = children.begin(); it != children.end(); it++)
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
    void BaseConnector::UpdateConfig(JsonObject config)
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

        rootConnector = new BaseConnector(hwName, cnf);

        std::string connectors = "{ \"" + rootConnector->GetName() + "\": {" + rootConnector->GetConfig() + "}}";

        // ToDo: Override file on change
        // File file = LittleFS.open(BaseConnector::configFilePath.c_str(), FILE_WRITE);
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
    void BaseConnector::InitConfig(std::string configFilePath)
    {
        BaseConnector::configFilePath = configFilePath;
        File file = LittleFS.open(BaseConnector::configFilePath.c_str(), FILE_READ);
        DynamicJsonDocument jsonDoc(32768);

        deserializeJson(jsonDoc, file);

        file.close();
        UpdateConfig(jsonDoc.as<JsonObject>());
    }
} // namespace ModelController
