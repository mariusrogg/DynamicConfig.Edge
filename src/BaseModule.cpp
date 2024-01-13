//!
//! @file BaseModule.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the base module
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseModule.hpp"
#include "WiFiHandler.hpp"
#include <sstream>
#include "LittleFS.h"
#include "Logger.hpp"

#include "Gain.hpp"
#include "SequenceProcessor.hpp"
#include "OnboardPWM.hpp"
#include "MQTTClient.hpp"

namespace ModelController
{
    //!
    //! @brief Root module
    //!
    BaseModule* BaseModule::rootModule = nullptr;
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
    //! @brief Set config to object and generate submodules
    //!
    void BaseModule::SetConfig(JsonObject config)
    {
        Logger::trace(GetPath() + "\t Config: " + ((JsonVariant)config).as<std::string>());
        for (JsonPair child : config)
        {
            if (child.value().is<JsonObject>())
            {
                Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                GenerateModule(child.key().c_str(), child.value(), this);
            }
        }
    }
    //!
    //! @brief Returns child of the module
    //!
    BaseModule* BaseModule::GetChild(std::string modulePath, ModuleType type, ModuleDataType dataType)
    {
        Logger::trace("BaseModule::GetChild(" + modulePath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Logger::trace("this->path: " + GetPath());
        //! @brief Trim '/' at start of path
        Utils::TrimStart(modulePath, "/");
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
    //! @brief Construct a new Module object
    //!
    BaseModule::BaseModule(std::string name, BaseModule* parent, bool createShortPath, ModuleType type, ModuleDataType dataType)
        : parent(parent),
        moduleType(type),
        moduleDataType(dataType)
    {
        this->name = Utils::Trim(name, "/");
        this->path = (this->parent != nullptr ? Utils::TrimEnd(this->parent->path, "/") : "") + "/" + this->name;
        Logger::trace("BaseModule::BaseModule(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        if (createShortPath)
        {
            this->shortPath = "/" + this->name;
        }
        else
        {
            this->shortPath = (this->parent != nullptr ? this->parent->shortPath : "") + "/" + this->name;
        }
        if (this->parent)
        {
            this->parent->children.push_back(this);
            Logger::debug("Added " + GetPath() + " to children of " + parent->GetPath());
        }
    }
    //!
    //! @brief Construct a new Module object
    //!
    BaseModule::BaseModule(std::string name, JsonObject config, BaseModule* parent, ModuleType type, ModuleDataType dataType)
        : BaseModule(name, parent, config["shortPath"].is<bool>() ? config["shortPath"].as<bool>() : false, type, dataType)
    {
        Logger::trace("BaseModule::BaseModule(" + name + ", json-config, " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        SetConfig(config);
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
    //! @brief Generates module from json
    //!
    BaseModule* BaseModule::GenerateModule(std::string name, JsonObject moduleConfig, BaseModule* parent)
    {
        Logger::trace("BaseModule::GenerateModule(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ")");
        BaseModule* module = nullptr;
        if (moduleConfig["type"].is<std::string>())
        {
            std::string type = moduleConfig["type"].as<std::string>();
            if (type == Gain::type)
            {
                module = new Gain(name, moduleConfig, parent);
            }
            else if (type == SequenceProcessor::type)
            {
                module = new SequenceProcessor(name, moduleConfig, parent);
            }
            else if (type == OnboardPWM::type)
            {
                module = new OnboardPWM(name, moduleConfig, parent);
            }
            else if (type == MQTTClient::type)
            {
                module = new MQTTClient(name, moduleConfig, parent);
            }
        }
        if (module == nullptr)
        {
            for (JsonPair child : moduleConfig)
            {
                if (child.value().is<JsonObject>())
                {
                    Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                    GenerateModule(name + "/" + child.key().c_str(), child.value(), parent);
                }
            }
        }
        return module;
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

        if (config["wifi"]["ssid"].isNull())
        {
            config["wifi"]["ssid"] = WiFiHandler::GetSSID();
        }
        if (config["wifi"]["password"].isNull())
        {
            config["wifi"]["password"] = WiFiHandler::GetPassword();
        }

        WiFiHandler::SetSSIDPassword(config["wifi"]["ssid"], config["wifi"]["password"]);


        if (rootModule != nullptr)
        {
            delete rootModule;
            rootModule = nullptr;
            Logger::trace("Deleted old rootModule");
        }
        rootModule = new BaseModule("");
        rootModule->SetConfig(config);

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
