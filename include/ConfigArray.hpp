//!
//! @file ConfigArray.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for accessing an array from config file
//!
//! @copyright Copyright (c) 2024
//!
#pragma once

#include "IConfigItem.hpp"
#include <vector>

namespace ModelController
{
    template <typename T>
    class ConfigArray : public IConfigItem<std::vector<T>>
    {
        public:
            //!
            //! @brief Construct a new Config Item object
            //!
            //! @param name Name of the config item
            //! @param parentConfig Parent config (where config items value is stored in)
            //! @param defaultValue Default value, to be set to config item, if no value is found in config
            //! @param parent Parent of the config item
            //!
            ConfigArray(std::string name, std::vector<T> defaultValue, BaseModule* parent = nullptr, bool deleteOnDefault = false)
                : IConfigItem<std::vector<T>>(name, defaultValue, parent, deleteOnDefault)
            {
                std::sort(defaultValue.begin(), defaultValue.end());
                // Set value from config
                std::optional<JsonArray> config = ConfigFile::GetConfig<JsonArray>(this->GetPath());
                if (config)
                {
                    SetValue(Utils::JsonArrayToArray<T>(config.value()));
                }
                else 
                {
                    SetValue(this->defaultValue);
                }
            }
            //!
            //! @brief Set the value of the config item
            //!
            //! @param value New config value
            //!
            virtual void SetValue(std::vector<T> value) override
            {
                std::sort(value.begin(), value.end());
                this->value = value;
                if (this->value == this->defaultValue && this->deleteOnDefault)
                {
                    ConfigFile::Remove(this->GetPath());
                }
                else
                {
                    std::optional<JsonArray> jsonArray = ConfigFile::GetConfig<JsonArray>(this->GetPath(), true);
                    if (jsonArray)
                    {
                        Utils::ArrayToJsonArray<T>(this->value, jsonArray.value());
                    }
                    else
                    {
                        Logger::error(this->GetPath() + ": No JsonArray returned from config");
                    }
                }
            }
            //!
            //! @brief Make assignment operator from IConfigItem available to set value
            //!
            using IConfigItem<std::vector<T>>::operator=;
    };
    
} // namespace ModelController
