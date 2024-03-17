//!
//! @file ConfigItem.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for accessing value from config file
//!
//! @copyright Copyright (c) 2024
//!
#pragma once

#include "IConfigItem.hpp"

namespace ModelController
{
    template <typename T>
    class ConfigItem : public IConfigItem<T>
    {
        public:
            //!
            //! @brief Construct a new Config Item object
            //!
            //! @param name Name of the config item
            //! @param defaultValue Default value, to be set to config item, if no value is found in config
            //! @param parent Parent of the config item
            //! @param deleteOnDefault True if config value is deleted, if set to default
            //!
            ConfigItem(std::string name, T defaultValue, BaseModule* parent = nullptr, bool deleteOnDefault = false)
                : IConfigItem<T>(name, defaultValue, parent, deleteOnDefault)
            {
                // Set value from config
                SetValue(ConfigFile::GetConfig<T>(this->GetPath()).value_or(this->defaultValue));
            }
            //!
            //! @brief Set the value of the config item
            //!
            //! @param value New config value
            //!
            virtual void SetValue(T value) override
            {
                this->value = value;
                if (this->value == this->defaultValue && this->deleteOnDefault)
                {
                    ConfigFile::Remove(this->GetPath());
                }
                else
                {
                    ConfigFile::SetConfig(this->GetPath(), value);
                }
            }
            //!
            //! @brief Make assignment operator from IConfigItem available to set value
            //!
            using IConfigItem<T>::operator=;
            //!
            //! @brief Mulitply config item value with another value
            //!
            //! @param value Value to multiply config item value with
            //! @return T ConfigItem.GetValue() * value
            //!
            T operator*(T value)
            {
                return this->GetValue() * value;
            }
    };
    //!
    //! @brief Mulitply config item value with another value
    //!
    //! @tparam T Type of the value and template type of the config item
    //! @param value Value to multiply config item value with
    //! @param item Config item, from which the value is used for multiplication
    //! @return T ConfigItem.GetValue() * value
    //!
    template <typename T>
    T operator*(T value, ConfigItem<T> item)
    {
        return item * value;
    }

} // namespace ModelController
