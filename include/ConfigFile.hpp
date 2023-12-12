//!
//! @file ConfigFile.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for handling access to json-config
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <string>
#include "Logger.hpp"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "EventHandling.hpp"
#include "Utils.hpp"

namespace ModelController
{
    class ConfigFile
    {
        private:
            //!
            //! @brief Delete ctor for creating pure static class
            //!
            ConfigFile() = delete;
            //!
            //! @brief Json document, where json config is stored
            //!
            static DynamicJsonDocument configDoc;

        public:
            //!
            //! @brief Path to the config file
            //!
            static std::string configFilePath;
            //!
            //! @brief Default config file path
            //!
            static constexpr const char* defaultConfigFile = "/Config.json";
            //!
            //! @brief Event raised if config was reloaded
            //!
            static Event<> ConfigReloaded;
            //!
            //! @brief (Re-)load the config from a file
            //!
            //! @param configFilePath Path to the config file
            //!
            static void Load(std::string configFilePath = defaultConfigFile);
            //!
            //! @brief Get the config at a specified path
            //!
            //! @param path Path to get config from
            //! @param create If true, element with path is created, if not yet existing
            //! @return JsonVariant Element at specified path
            //!
            static JsonVariant GetConfig(std::string path, bool create = false);
            //!
            //! @brief Set a value to the config
            //!
            //! @tparam T Type of the value
            //! @param path Path to set value to
            //! @param value Value to be set
            //!
            template<typename T>
            static void SetConfig(std::string path, T value)
            {
                GetConfig(path, true).set(value);
                configDoc.garbageCollect();
                Save();
            }
            //!
            //! @brief Remove elements from json
            //!
            //! @param path Path to element to be removed
            //! @param save True if config is saved after removing
            //!
            static void Remove(std::string path, bool save = true);
            //!
            //! @brief Save config
            //!
            //! @return true If sucessfull
            //! @return false If unsecessfull
            //!
            static bool Save();
            //!
            //! @brief Get the content of the config as prettyfied json
            //!
            //! @return std::string Config as string
            //!
            static std::string GetConfigFile();
    };
} // namespace ModelController
