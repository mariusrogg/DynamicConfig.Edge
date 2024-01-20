//!
//! @file BaseModule.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of a module (e.g. PWM-Channel, PID-Controller, ...)
//!
//! @copyright Copyright (c) 2023
//!
#pragma once

#include <ArduinoJson.h>
#include <string>
#include <vector>
#include "Utils.hpp"
#include "Logger.hpp"

namespace ModelController
{
    class BaseModule
    {
        public:
            //!
            //! @brief Type of the module
            //!
            enum class ModuleType
            {
                eUndefined,      //!< Type is not defined
                eInput,          //!< Type is input
                eOutput,         //!< Type is output
                eNone,           //!< Type is neither input nor output
            };
            //!
            //! @brief Get ModuleType as string
            //!
            //! @param type ModuleType to get
            //! @return std::string Name of the type
            //!
            static std::string TypeToString(ModuleType type);
            //!
            //! @brief Data type of the connected variable
            //!
            enum class ModuleDataType
            {
                eUndefined, //!< Type is not defined
                eNone,      //!< Module has no type
                eDouble,    //!< Data type is double
                eFloat,     //!< Data type is float
                eString,    //!< Data type is std::string
                eInt8,      //!< Data type is signed 8 bit integer
                eInt16,     //!< Data type is signed 16 bit integer
                eInt32,     //!< Data type is signed 32 bit integer
                eInt64,     //!< Data type is signed 64 bit integer
                eUInt8,     //!< Data type is unsigned 8 bit integer
                eUInt16,    //!< Data type is unsigned 16 bit integer
                eUInt32,    //!< Data type is unsigned 32 bit integer
                eUInt64,    //!< Data type is unsigned 64 bit integer
                eBool,      //!< Data type is boolean
            };
            //!
            //! @brief Get ModuleDataType as string
            //!
            //! @param dataType ModuleDataType to get
            //! @return std::string Name of the dataType
            //!
            static std::string DataTypeToString(ModuleDataType dataType);//!
            //!
            //! @brief Get the data type by typeid
            //!
            //! @param typeInfo Usually pass typeid(object) or typeid(Type)
            //! @return ModuleDataType Matching data type
            //!
            static ModuleDataType GetDataTypeById(const std::type_info& typeInfo);
            //!
            //! @brief Get the Type of the module
            //!
            //! @return ModuleType Type of the object
            //!
            ModuleType GetType() const;
            //!
            //! @brief Get the DataType of the module
            //!
            //! @return ModuleDataType DataType of the object
            //!
            ModuleDataType GetDataType() const;
        private:
            //!
            //! @brief Parent of the module object
            //!
            BaseModule* parent = nullptr;
            //!
            //! @brief Children of the module object
            //!
            std::vector<BaseModule*> children;
            //!
            //! @brief Path of the module object
            //!
            std::string path;
            //!
            //! @brief Short path of the module object
            //!
            std::string shortPath;
            //!
            //! @brief Name of the module object
            //!
            std::string name;
            //!
            //! @brief Default config file path
            //!
            static std::string configFilePath;

        protected:
            //!
            //! @brief Type of the module
            //!
            ModuleType moduleType = ModuleType::eUndefined;
            //!
            //! @brief Data-Type of the module
            //!
            ModuleDataType moduleDataType = ModuleDataType::eUndefined;
            //!
            //! @brief Set the config to the module
            //!
            //! @param config Config to be set
            //!
            virtual void SetConfig(JsonObject config);
            //!
            //! @brief Get child of the object
            //!
            //! @param modulePath Path of the child
            //! @param type ModuleType of the child
            //! @param dataType ModuleDataType of the child
            //! @return BaseModule* Child, nullptr if not found
            //!
            virtual BaseModule* GetChild(std::string modulePath, ModuleType type = ModuleType::eUndefined, ModuleDataType dataType = ModuleDataType::eUndefined);
        public:
            //!
            //! @brief Root module of the hardware configuration
            //!
            static BaseModule* rootModule; // ToDo: Make RootModule class (preferably singleton)
            //!
            //! @brief Construct a new module object
            //!
            //! @param name Name of the module
            //! @param config Config of the module
            //! @param parent Parent of the module (normally pass this)
            //! @param type Type of the module
            //! @param dataType DataType of the module
            //!
            BaseModule(std::string name, JsonObject config, BaseModule* parent = nullptr, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Construct a new module object
            //!
            //! @param name Name of the module
            //! @param parent Parent of the module (normally pass this)
            //! @param createShortPath True if short path is created from actual object
            //! @param type Type of the module
            //! @param dataType DataType of the module
            //!
            BaseModule(std::string name, BaseModule* parent = nullptr, bool createShortPath = false, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Destruction of the Base Module object
            //!
            virtual ~BaseModule();
            //!
            //! @brief Generate module from json
            //!
            //! @param moduleConfig Config of the module
            //! @return BaseModule* Created module
            //!
            static BaseModule* GenerateModule(std::string name, JsonObject moduleConfig, BaseModule* parent = nullptr);
            //!
            //! @brief Default config file path
            //!
            static constexpr const char* defaultConfigFile = "/Config.json";
            //!
            //! @brief Get the Parent of the object
            //!
            //! @return BaseModule* Parent of the object
            //!
            BaseModule* GetParent() const;
            //!
            //! @brief Get the path of the object
            //!
            //! @return std::string Path of the object
            //!
            std::string GetPath() const;
            //!
            //! @brief Get the short path of the object
            //!
            //! @return std::string Short path of the object
            //!
            std::string GetShortPath() const;
            //!
            //! @brief Get the name of the object
            //!
            //! @return std::string Name of the object
            //!
            std::string GetName() const;
            //!
            //! @brief Get a module by path
            //!
            //! @param modulePath Path of the module
            //! @return T* BaseModule with path
            //!
            template<class T>
            T* GetChildModule(std::string modulePath, ModuleType type = ModuleType::eUndefined, ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                return dynamic_cast<T*>(GetChild(modulePath, type, dataType));
            }
            //!
            //! @brief Get a module by path
            //!
            //! @tparam T Class type of the module
            //! @param type Type of the module
            //! @param dataType Datatype of the module
            //! @param modulePath Path of the module
            //! @return T* BaseModule with path
            //!
            template<class T>
            static T* GetModule(std::string modulePath, ModuleType type = ModuleType::eUndefined, ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                //! Remove "/" from beginning and end of the path
                modulePath = Utils::Trim(modulePath, "/");
                if (rootModule == nullptr)
                {
                    Logger::trace("Root module not created yet");
                }
                return rootModule == nullptr ? nullptr : rootModule->GetChildModule<T>(modulePath, type, dataType);
            }
            //!
            //! @brief Update hardware configuration
            //!
            //! @param config Json object with new hardware config
            //!
            static void UpdateConfig(JsonObject config);
            //!
            //! @brief Get the config of the connector
            //!
            //! @return string Config created
            //!
            virtual std::string GetConfig();
    };
} // namespace ModelController
