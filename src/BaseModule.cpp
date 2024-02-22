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
    //! @brief Path to the api
    //!
    std::string BaseModule::apiPath = "/Connectors/mqtt";
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
    //! @brief Delete children of the current module
    //!
    void BaseModule::Delete()
    {
        Logger::debug("Deleting module " + GetPath());
        for (BaseModule* child : children)
        {
            child->Delete();
        }
    }
    //!
    //! @brief Returns child of the module
    //!
    BaseModule* BaseModule::GetChild(std::string modulePath, ModuleType type, ModuleDataType dataType, bool recursive)
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
                    if (sizePath >= sizeName + 1 && recursive)
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
    //! @brief Returns GetChild method without recursion
    //!
    BaseModule* BaseModule::GetDirectChild(std::string childPath)
    {
        return GetChild(childPath, ModuleType::eUndefined, ModuleDataType::eUndefined, false);
    }
    //!
    //! @brief Returns this, if no child exists mathing to child path, else returns child
    //!
    BaseModule* BaseModule::GetFinalMatchingChild(std::string childPath)
    {
        Logger::trace("GetFinalMatchingChild(" + childPath + ")");
        childPath = Utils::TrimStart(childPath, "/");
        BaseModule* module = GetDirectChild(childPath);
        if (module == nullptr)
        {
            module = this;
        }
        else
        {
            module = module->GetFinalMatchingChild(Utils::TrimStart(childPath, module->GetName(), 1));
        }
        return module;
    }
    //!
    //! @brief Checks type of object and parameter type match and add object's and matching children's path to list
    //!
    std::vector<std::string> BaseModule::GetContainers(std::string type)
    {
        Logger::trace(GetPath() + "->BaseModule::GetContainers(" + type  + ")");
        std::vector<std::string> containers;
        BaseContainer* container = dynamic_cast<BaseContainer*>(this);
        if (container != nullptr)
        {
            if (type.empty())
            {
                Logger::trace("Found container");
                containers.push_back(GetPath());
            }
            else if (dynamic_cast<Gain*>(this) && type == Gain::type)
            {
                Logger::trace("Found container");
                containers.push_back(GetPath());
            }
            else if (dynamic_cast<SequenceProcessor*>(this) && type ==SequenceProcessor::type)
            {
                Logger::trace("Found container");
                containers.push_back(GetPath());
            }
            else if (dynamic_cast<OnboardPWM*>(this) && type == OnboardPWM::type)
            {
                Logger::trace("Found container");
                containers.push_back(GetPath());
            }
            else if (dynamic_cast<MQTTClient*>(this) && type == MQTTClient::type)
            {
                Logger::trace("Found container");
                containers.push_back(GetPath());
            }
        }
        for (BaseModule* child : children)
        {
            std::vector<std::string> childContainers = child->GetContainers(type);
            containers.insert(containers.end(), childContainers.begin(), childContainers.end());
        }
        return containers;
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
            parent->children.erase(remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
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
    //! @brief Get module with path and call delete on this module
    //!
    void BaseModule::Delete(std::string path)
    {
        Logger::trace("BaseModule::Delete(" + path + ")");
        if (BaseModule* module = GetModule<BaseModule>(path))
        {
            module->Delete();
        }
    }
    std::string BaseModule::Set(std::string path, std::string config)
    {
        Logger::trace("BaseModule::Set(" + path + ", " + config + ")");
        std::string errorMessage = "";
        JsonDocument configDoc;
        ArduinoJson::DeserializationError error = deserializeJson(configDoc, config);

        if (!error)
        {
            if (configDoc.is<JsonObject>())
            {
                // Remove '/' at beginning of the path
                path = Utils::TrimStart(path, "/");
                // Delete object at path (if exists)
                Delete(path);
                // Get parent of the module to be created
                BaseModule* parent = GetFinalMatchingModule<BaseModule>(path);
                // Path of the parent (without trailing slash)
                std::string parentPath = Utils::TrimStart(parent->GetPath(), "/");
                // Name of the child
                std::string childName = Utils::TrimStart(path, parentPath);
                // Delete children, which will be children of created module
                for (std::vector<BaseModule*>::reverse_iterator it = parent->children.rbegin(); it != parent->children.rend(); it++)
                {
                    if (Utils::StartsWith((*it)->GetName(), childName))
                    {
                        (*it)->Delete();
                    }
                }
                // ToDo: Set config for ConfigItems and other not BaseContainer stuff
                // Set config to config doc (for persistence)
                ConfigFile::SetConfig(path, configDoc.as<JsonObject>());
                BaseModule::GenerateModule(childName, configDoc.as<JsonObject>(), parent);
            }
            else
            {
                errorMessage = "NoObjectType";
            }
        }
        else
        {
            errorMessage = error.c_str();
        }


        return errorMessage;
    }
    //!
    //! @brief
    //!
    std::vector<std::string> BaseModule::GetContainers(std::string path, std::string type)
    {
        BaseModule* module = GetFinalMatchingModule<BaseModule>(path);
        std::vector<std::string> containers;
        if (module == nullptr)
        {
            Logger::trace("Module not found");
        }
        else
        {
            path = '/' + Utils::Trim(path, "/");
            //! Module with exact path found -> return GetContainers of module
            if (module->GetPath() == path)
            {
                containers = module->GetContainers(type);
            }
            //! Module with exact path not found -> container added from modules starting with path
            else
            {
                path = path + '/';
                for (BaseModule* child : module->children)
                {
                    if (Utils::StartsWith(child->GetPath(), path))
                    {
                        std::vector<std::string> childContainers = child->GetContainers(type);
                        containers.insert(containers.end(), childContainers.begin(), childContainers.end());
                    }
                }
            }
        }
        return containers;
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

        if (!config["APIPath"].is<std::string>())
        {
            config["APIPath"] = apiPath;
        }
        else
        {
            apiPath = config["APIPath"].as<std::string>();
        }
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
