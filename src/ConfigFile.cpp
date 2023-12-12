//!
//! @file ConfigFile.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the config file
//!
//! @copyright Copyright (c) 2023
//!
#include "ConfigFile.hpp"

namespace ModelController
{
    //!
    //! @brief Path to the configuration path
    //!
    std::string ConfigFile::configFilePath = ConfigFile::defaultConfigFile;
    //!
    //! @brief Json config document
    //!
    DynamicJsonDocument ConfigFile::configDoc(32768);
    //!
    //! @brief Event raised, if config was reloaded
    //!
    Event<> ConfigFile::ConfigReloaded;
    //!
    //! @brief Event raised, if config was changed
    //!
    Event<std::string> ConfigFile::ConfigChanged;
    //!
    //! @brief Event raised, if config was deleted
    //!
    Event<std::string> ConfigFile::ConfigDeleted;
    //!
    //! @brief Open config file and write content to configDoc
    //!
    void ConfigFile::Load(std::string configFilePath)
    {
        Logger::trace("BaseModule::InitConfig(" + configFilePath + ")");
        ConfigFile::configFilePath = configFilePath;
        configDoc.clear();
        File file = LittleFS.open(ConfigFile::configFilePath.c_str(), FILE_READ);

        deserializeJson(configDoc, file);

        file.close();

        ConfigReloaded();
    }
    //!
    //! @brief Parse path and return json element on given path
    //!
    JsonVariant ConfigFile::GetConfig(std::string path, bool create)
    {
        Logger::trace("ConfigFile::GetConfig(" + path + ")");
        JsonVariant result;
        path = Utils::Trim(path, "/");

        JsonVariant jVariant = configDoc.as<JsonVariant>();
        JsonVariant oldJVariant;
        if (path.empty())
        {
            result = jVariant;
        }
        else if (jVariant.is<JsonObject>())
        {
            while (!path.empty() && result.isNull() && oldJVariant != jVariant)
            {
                oldJVariant = jVariant;
                JsonObject jObject = jVariant.as<JsonObject>();
                for (JsonPair child : jObject)
                {
                    std::string name = child.key().c_str();
                    size_t posName = path.find(name);
                    size_t sizeName = name.size();
                    size_t sizePath = path.size();

                    if (sizePath >= sizeName && posName == 0)
                    {
                        size_t posSlash = path.find_first_of('/', sizeName);
                        Logger::trace("Position of first slash in path: " + std::to_string(posSlash));
                        //! @brief Check if name is adressed by path (name eqal path or path starts with name + '/')
                        if (sizeName == sizePath || posSlash == sizeName)
                        {
                            //! @brief Check if size of path is longer than name -> searched element is grandchild
                            if (sizePath >= sizeName + 1)
                            {
                                path = path.substr(sizeName + 1);
                                Logger::trace("Sub path: " + path);
                                jVariant = child.value();
                                break;
                            }
                            //! @brief Path is equal to name -> searched element is child
                            else
                            {
                                Logger::trace("Child found: " + child.value().as<std::string>());
                                result = child.value();
                                path = "";
                            }
                        }
                    }
                }
            }
        }
        //! @brief Json element was not found, new one is created with given path
        if (create && (configDoc.isNull() || oldJVariant == jVariant))
        {
            while (!path.empty())
            {
                Logger::trace("Path: " + path);
                // Trim "/" at beginning
                path = path.substr(path.find_first_not_of("/"));
                Logger::trace("Path after trim: " + path);
                size_t posSlash = path.find_first_of('/');
                if (path.size() > 0)
                {
                    Logger::trace("Subpath: " + path.substr(0, posSlash));
                    jVariant[path.substr(0, posSlash)].set(nullptr);
                    jVariant = jVariant[path.substr(0, posSlash)];
                    if (posSlash == std::string::npos)
                    {
                        result = jVariant;
                        path = "";
                    }
                    else
                    {
                        path = path.substr(posSlash);
                    }
                }
            }
        }
        return result;
    }
    //!
    //! @brief Remove element from config
    //!
    void ConfigFile::Remove(std::string path, bool save)
    {
        Logger::trace("ConfigFile::Remove(" + path + ", " + std::to_string(save) + ")");
        Logger::trace("path: " + path);
        path = Utils::Trim(path, "/");
        path = Utils::TrimStart(path, "root");
        path = Utils::TrimStart(path, "/");
        std::string name;
        if (path.find_last_of('/') == std::string::npos)
        {
            name = path;
            path = "";
        }
        else
        {
            name = path.substr(path.find_last_of('/') + 1);
            path = path.substr(0, path.find_last_of("/"));
        }
        Logger::trace("name: " + name);
        Logger::trace("path: " + path);
        Logger::trace(GetConfig(path).as<std::string>());
        if (!name.empty())
        {
            JsonVariant parentConfig = GetConfig(path, false);
            if (parentConfig.is<JsonObject>())
            {
                JsonObject objParentConfig = parentConfig.as<JsonObject>();
                objParentConfig.remove(name);
                configDoc.garbageCollect();
                //! @brief Remove parent element, if it does not contain more children than the deleted one
                if (path.find_first_not_of('/') != std::string::npos && (objParentConfig.isNull() || (objParentConfig.size() == 0)))
                {
                    Remove(path, false);
                }
                //! @brief If parent does not need to be deleted (has more children), event is raised, informing, that part of the config was deleted
                else
                {
                    ConfigDeleted(path + "/" + name);
                }
                //! @brief Save after removing
                if (save)
                {
                    Save();
                }
            }
        }
    }
    //!
    //! @brief Open file in write mode and write config to file, if file was opened sucessfully
    //!
    bool ConfigFile::Save()
    {
        File file = LittleFS.open(configFilePath.c_str(), FILE_WRITE);
        if (!file)
        {
            return false;
        }
        serializeJson(configDoc, file);
        file.close();
        return true;
    }
    //!
    //! @brief Return prettyfied json
    //!
    std::string ConfigFile::GetConfigFile()
    {
        std::string config;
        serializeJsonPretty(configDoc, config);
        return config;
    }

} // namespace ModelController
