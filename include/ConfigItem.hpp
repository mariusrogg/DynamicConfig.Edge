#pragma once

#include "BaseModule.hpp"
#include "ConfigFile.hpp"

namespace ModelController
{
    template <typename T>
    class ConfigItem : public BaseModule
    {
        private:
            T value;
            T defaultValue;
        protected:
            virtual void Delete() override
            {
                value = defaultValue;
                ConfigFile::SetConfig(GetPath(), value);
            }
            virtual void Update(JsonVariant config) override
            {
                if (config.is<T>())
                {
                    value = config.as<T>();
                    ConfigFile::SetConfig(GetPath(), value);
                }
                else
                {
                    ConfigFile::SetConfig(GetPath(), value);
                }
            }
        public:
            ConfigItem(std::string name, JsonObject parentConfig, T defaultValue, BaseModule* parent = nullptr)
                : BaseModule(name, parent, false, ModuleType::eNone, GetDataTypeById(typeid(T))),
                value(defaultValue),
                defaultValue(defaultValue)
            {
                Update(parentConfig[name]);
            }
            T GetValue() const
            {
                return value;
            }
    };

} // namespace ModelController
