//!
//! @file IConfigItem.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface class for accessing value from config file
//!
//! @copyright Copyright (c) 2024
//!
#pragma once

#include "BaseModule.hpp"
#include "ConfigFile.hpp"

namespace ModelController
{
    template <typename T>
    class IConfigItem : public BaseModule
    {
        protected:
            //!
            //! @brief Actual value of the config item
            //!
            T value;
            //!
            //! @brief Default value of the config item
            //!
            T defaultValue;
            //!
            //! @brief True if value should be deleted, if set to default
            //!
            bool deleteOnDefault;
            //!
            //! @brief Delete ConfigItem (reset to default)
            //!
            virtual void Delete() override
            {
                SetValue(defaultValue);
            }

        public:
            //!
            //! @brief Construct a new Config Item object
            //!
            //! @param name Name of the config item
            //! @param defaultValue Default value, to be set to config item, if no value is found in config
            //! @param parent Parent of the config item
            //! @param deleteOnDefault True if config value is deleted, if set to default
            //!
            IConfigItem(std::string name, T defaultValue, BaseModule* parent = nullptr, bool deleteOnDefault = false)
                : BaseModule(name, parent, ModuleType::eNone, GetDataTypeById(typeid(T))),
                value(defaultValue),
                defaultValue(defaultValue),
                deleteOnDefault(deleteOnDefault)
            {
            }
            //!
            //! @brief Set the value of the config item
            //!
            //! @param value New config value
            //!
            virtual void SetValue(T value) = 0;
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
    };
} // namespace ModelController
