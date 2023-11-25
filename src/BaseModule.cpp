//!
//! @file BaseModule.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the base module
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseModule.hpp"
#include "WiFiHandler.hpp"
#include "BaseConnector.hpp"
#include "BaseProcessor.hpp"
#include <sstream>
#include "LittleFS.h"
#include "Logger.hpp"

namespace ModelController
{
    //!
    //! @brief Switch through possible types and return type as string
    //!
    std::string BaseModule::TypeToString(ModuleType type)
    {
        std::string name;
        switch (type)
        {
            case ModuleType::eUndefined:
                name = "Undefined";
                break;
            case ModuleType::eInput:
                name = "Input";
                break;
            case ModuleType::eOutput:
                name = "Output";
                break;
            case ModuleType::eNone:
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
    std::string BaseModule::DataTypeToString(ModuleDataType dataType)
    {
        std::string name;
        switch (dataType)
        {
            case ModuleDataType::eUndefined:
                name = "Undefined";
                break;
            case ModuleDataType::eNone:
                name = "None";
                break;
            case ModuleDataType::eDouble:
                name = "Double";
                break;
            case ModuleDataType::eFloat:
                name = "Float";
                break;
            case ModuleDataType::eString:
                name = "String";
                break;
            case ModuleDataType::eInt8:
                name = "Int8";
                break;
            case ModuleDataType::eInt16:
                name = "Int16";
                break;
            case ModuleDataType::eInt32:
                name = "Int32";
                break;
            case ModuleDataType::eInt64:
                name = "Int64";
                break;
            case ModuleDataType::eUInt8:
                name = "UInt8";
                break;
            case ModuleDataType::eUInt16:
                name = "UInt16";
                break;
            case ModuleDataType::eUInt32:
                name = "UInt32";
                break;
            case ModuleDataType::eUInt64:
                name = "UInt64";
                break;
            case ModuleDataType::eBool:
                name = "Bool";
                break;
            default:
                break;
        }
        return name;
    }
    //!
    //! @brief Check possible type infos and return corresponding data type
    //!
    BaseModule::ModuleDataType BaseModule::GetDataTypeById(const std::type_info& typeInfo)
    {
        ModuleDataType type = ModuleDataType::eUndefined;
        if (typeInfo.hash_code() == typeid(double).hash_code())
        {
            type = ModuleDataType::eDouble;
        }
        else if (typeInfo.hash_code() == typeid(float).hash_code())
        {
            type = ModuleDataType::eFloat;
        }
        else if (typeInfo.hash_code() == typeid(std::string).hash_code())
        {
            type = ModuleDataType::eString;
        }
        else if (typeInfo.hash_code() == typeid(int8_t).hash_code())
        {
            type = ModuleDataType::eInt8;
        }
        else if (typeInfo.hash_code() == typeid(int16_t).hash_code())
        {
            type = ModuleDataType::eInt16;
        }
        else if (typeInfo.hash_code() == typeid(int32_t).hash_code())
        {
            type = ModuleDataType::eInt32;
        }
        else if (typeInfo.hash_code() == typeid(int64_t).hash_code())
        {
            type = ModuleDataType::eInt64;
        }
        else if (typeInfo.hash_code() == typeid(uint8_t).hash_code())
        {
            type = ModuleDataType::eUInt8;
        }
        else if (typeInfo.hash_code() == typeid(uint16_t).hash_code())
        {
            type = ModuleDataType::eUInt16;
        }
        else if (typeInfo.hash_code() == typeid(uint32_t).hash_code())
        {
            type = ModuleDataType::eUInt32;
        }
        else if (typeInfo.hash_code() == typeid(uint64_t).hash_code())
        {
            type = ModuleDataType::eUInt64;
        }
        else if (typeInfo.hash_code() == typeid(boolean).hash_code() || typeInfo.hash_code() == typeid(bool).hash_code())
        {
            type = ModuleDataType::eBool;
        }
        return type;
    }
    //!
    //! @brief Returns ModuleType
    //!
    BaseModule::ModuleType BaseModule::GetType() const
    {
        return moduleType;
    }
    //!
    //! @brief Returns ModuleDataType
    //!
    BaseModule::ModuleDataType BaseModule::GetDataType() const
    {
        return moduleDataType;
    }
    //!
    //! @brief Path to the configuration path
    //!
    std::string BaseModule::configFilePath = BaseModule::defaultConfigFile;
    //!
    //! @brief Returns child of the module
    //!
    BaseModule* BaseModule::GetChild(std::string modulePath, ModuleType type, ModuleDataType dataType)
    {
        Logger::trace("BaseModule::GetChild(" + modulePath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Logger::trace("this->path: " + GetPath());
        //! @brief Trim '/' at start of path
        modulePath = modulePath.substr(modulePath.find_first_not_of('/'));
        Logger::trace("Module path without trailing slashs: " + modulePath);
        BaseModule* module = nullptr;
        //! @brief Iterate through children
        for (BaseModule* child : children)
        {
            std::string name = child->GetName();
            size_t posName = modulePath.find(name);
            size_t sizeName = name.size();
            size_t sizePath = modulePath.size();
            Logger::trace("Child name: " + std::string(name));
            Logger::trace("Position of childs name in path: " + std::to_string(posName));
            Logger::trace("Size of childs name: " + std::to_string(sizeName));
            Logger::trace("Size of path: " + std::to_string(sizePath));

            //! @brief Check if path starts with name of child
            if (sizePath >= sizeName && posName == 0)
            {
                size_t posSlash = modulePath.find_first_of('/', child->GetName().size());
                Logger::trace("Position of first slash in path: " + std::to_string(posSlash));
                //! @brief Check if name is adressed by path (name eqal path or path starts with name + '/')
                if (sizeName == sizePath || posSlash == sizeName)
                {
                    //! @brief Check if size of path is longer than name -> searched element is grandchild
                    if (sizePath >= sizeName + 1)
                    {
                        std::string subPath = modulePath.substr(child->GetName().size() + 1);
                        Logger::trace("Sub path: " + subPath);
                        module = child->GetChild(subPath, type, dataType);
                    }
                    //! @brief Path is equal to name -> searched element is child
                    else
                    {
                        //! @brief Check if type and data type of child are equal to found childs type and data type
                        if ((type == ModuleType::eUndefined || child->GetType() == type)
                            && (dataType == ModuleDataType::eUndefined || child->GetDataType() == dataType))
                        {
                            Logger::trace("Child found: " + child->GetPath());
                            module = child;
                        }
                    }
                    //! @brief Break loop if searched module is found
                    if (module != nullptr)
                    {
                        break;
                    }
                }
            }
        }
        return module;
    }
    //!
    //! @brief Initialize properties and add this to parents children (if not already done)
    //!
    void BaseModule::Initialize(std::string name, BaseModule* parent, bool createShortPath, ModuleType type, ModuleDataType dataType)
    {
        Logger::trace("BaseModule::Initialize(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        if (this->parent == nullptr)
        {
            this->parent = parent;
        }
        if (this->moduleType == ModuleType::eUndefined)
        {
            this->moduleType = type;
        }
        if (this->moduleDataType == ModuleDataType::eUndefined)
        {
            this->moduleDataType = dataType;
        }
        this->name = name.substr(name.find_first_not_of("/"));
        this->path = (this->parent != nullptr ? this->parent->path : "") + "/" + this->name;
        if (createShortPath)
        {
            this->shortPath = "/" + this->name;
        }
        else
        {
            this->shortPath = (this->parent != nullptr ? this->parent->shortPath : "") + "/" + this->name;
        }
        if (this->parent && std::find(this->parent->children.begin(), this->parent->children.end(), this) == this->parent->children.end())
        {
            this->parent->children.push_back(this);
            Logger::debug("Added " + GetPath() + " to children of " + parent->GetPath());
        }
    }
    //!
    //! @brief Root module
    //!
    BaseModule* BaseModule::rootModule = nullptr;
    //!
    //! @brief Default ctor
    //!
    BaseModule::BaseModule()
    { }
    //!
    //! @brief Construct a new Module object
    //!
    BaseModule::BaseModule(std::string name, BaseModule* parent, bool createShortPath, ModuleType type, ModuleDataType dataType)
    {
        Logger::trace("BaseModule::BaseModule(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Initialize(name, parent, createShortPath, type, dataType);
    }
    //!
    //! @brief Destruction the module object
    //!
    BaseModule::~BaseModule()
    {
        Logger::trace("BaseModule::~BaseModule() - " + path);
        if (parent)
        {
            remove(parent->children.begin(), parent->children.end(), this);
        }
    }
    //!
    //! @brief Returns parent of the actual object
    //!
    BaseModule* BaseModule::GetParent() const
    {
        return parent;
    }
    //!
    //! @brief Returns path of the actual object
    //!
    std::string BaseModule::GetPath() const
    {
        return path;
    }
    //!
    //! @brief Returns short path of the actual object
    //!
    std::string BaseModule::GetShortPath() const
    {
        return shortPath;
    }
    //!
    //! @brief Returns name of the actual object
    //!
    std::string BaseModule::GetName() const
    {
        return name;
    }
    //!
    //! @brief Update config of the controller
    //!
    void BaseModule::UpdateConfig(JsonObject config)
    {
        Logger::info("Updating config");
        if (rootModule != nullptr)
        {
            delete rootModule;
            rootModule = nullptr;
            Logger::trace("Deleted old rootModule");
        }
        rootModule = new BaseModule("root");

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


        if (BaseConnector::rootConnector != nullptr)
        {
            delete BaseConnector::rootConnector;
            BaseConnector::rootConnector = nullptr;
            Logger::trace("Deleted old rootConnector");
        }
        std::string connectorsName = "Connectors";
        if (cnf[connectorsName.c_str()].is<JsonObject>())
        {
            BaseConnector::rootConnector = new BaseConnector(connectorsName, cnf[connectorsName.c_str()].as<JsonObject>(), rootModule);
        }

        if (BaseProcessor::rootProcessor != nullptr)
        {
            delete BaseProcessor::rootProcessor;
            BaseProcessor::rootProcessor = nullptr;
            Logger::trace("Deleted old rootProcessor");
        }
        std::string processorsName = "Processors";
        if (cnf[processorsName.c_str()].is<JsonObject>())
        {
            BaseProcessor::rootProcessor = new BaseProcessor(processorsName, cnf[processorsName.c_str()].as<JsonObject>(), rootModule);
        }

        // std::string connectors = "{ \"" + rootConnector->GetName() + "\": {" + rootConnector->GetConfig() + "}}";

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
    void BaseModule::InitConfig(std::string configFilePath)
    {
        Logger::trace("BaseModule::InitConfig(" + configFilePath + ")");
        BaseModule::configFilePath = configFilePath;
        File file = LittleFS.open(BaseModule::configFilePath.c_str(), FILE_READ);
        DynamicJsonDocument jsonDoc(32768);

        deserializeJson(jsonDoc, file);

        file.close();
        UpdateConfig(jsonDoc.as<JsonObject>());
    }
    //!
    //! @brief Get config of the connector and it's children
    //!
    std::string BaseModule::GetConfig()
    {
        std::stringstream config;
        for (std::vector<BaseModule*>::const_iterator it = children.begin(); it != children.end(); it++)
        {
            if (it != children.begin())
            {
                config << ",";
            }
            config << "\"" << (*it)->GetName() << "\": {" << (*it)->GetConfig() << "}";
        }
        return config.str();
    }
} // namespace ModelController
