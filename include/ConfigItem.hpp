//!
//! @file ConfigItem.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for accessing value from config file
//!
//! @copyright Copyright (c) 2024
//!
#pragma once

#include "BaseModule.hpp"
#include "ConfigFile.hpp"

namespace ModelController
{
    template <typename T>
    class ConfigItem : public BaseModule
    {
        private:
            //!
            //! @brief Actual value of the config item
            //!
            T value;
            //!
            //! @brief Default value of the config item
            //!
            T defaultValue;
        public:
            //!
            //! @brief Construct a new Config Item object
            //!
            //! @param name Name of the config item
            //! @param parentConfig Parent config (where config items value is stored in)
            //! @param defaultValue Default value, to be set to config item, if no value is found in config
            //! @param parent Parent of the config item
            //!
            ConfigItem(std::string name, JsonObject parentConfig, T defaultValue, BaseModule* parent = nullptr)
                : BaseModule(name, parent, false, ModuleType::eNone, GetDataTypeById(typeid(T))),
                value(defaultValue),
                defaultValue(defaultValue)
            {
                if (parentConfig[name].is<T>())
                {
                    SetValue(parentConfig[name].as<T>());
                }
            }
            //!
            //! @brief Set the value of the config item
            //!
            //! @param value New config value
            //!
            void SetValue(T value)
            {
                this->value = value;
                ConfigFile::SetConfig(GetPath(), value);
            }
            //!
            //! @brief Get the value of the config item
            //!
            //! @return T Config value
            //!
            T GetValue() const
            {
                return value;
            }
            //!
            //! @brief Assignment operator to set value
            //!
            //! @param value Value to be set
            //!
            void operator=(T value)
            {
                SetValue(value);
            }
            //!
            //! @brief Operator T to return value
            //!
            //! @return T Value of the config item
            //!
            operator T()
            {
                return GetValue();
            }
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
