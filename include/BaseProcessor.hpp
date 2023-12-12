//!
//! @file BaseProcessor.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of Hardware processors
//!
//! @copyright Copyright (c) 2022
//!
//!
#pragma once

#include <ArduinoJson.h>
#include <string>
#include "BaseModule.hpp"
#include "Logger.hpp"
#include "ConfigFile.hpp"
#include "IBaseProcessor.hpp"

namespace ModelController
{
    // class RootProcessor;
    template<typename Derived>
    class BaseProcessor : public IBaseProcessor
    {
        protected:
            virtual void Delete() override
            {
                ConfigFile::Remove(GetPath());
                delete this;
            }
            virtual void Update(JsonVariant config) override
            {
                if (!config["type"].is<std::string>() || config["type"].as<std::string>() != Derived::GetProcessorType())
                {
                    Delete();
                }
                else
                {
                    BaseModule::Update(config);
                }
            }
            virtual void Create(std::string name, JsonObject childConfig) override
            {
                GenerateProcessor(name, childConfig, this);
            }
        public:
            //!
            //! @brief Construct a new Processor object
            //!
            //! @param name Name of the processor
            //! @param config Config of the processor
            //! @param parent Parent of the Processor (normally pass this)
            //! @param type Type of the processor
            //! @param dataType DataType of the processor
            //!
            BaseProcessor(std::string name, JsonObject config, BaseModule* parent = nullptr, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Construct a new Processor object
            //!
            //! @param name Name of the processor
            //! @param parent Parent of the Processor (normally pass this)
            //! @param createShortPath True if short path is created from actual object
            //! @param type Type of the processor
            //! @param dataType DataType of the processor
            //!
            BaseProcessor(std::string name, BaseModule* parent = nullptr, bool createShortPath = false, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Destroy the Processor object
            //!
            virtual ~BaseProcessor();
            //!
            //! @brief Generate processor from json
            //!
            //! @param processorConfig Config of the processor
            //! @return IBaseProcessor* Created processor
            //!
            static IBaseProcessor* GenerateProcessor(std::string name, JsonObject processorConfig, IBaseProcessor* parent = nullptr);
            //!
            //! @brief Get a processor by path
            //!
            //! @tparam T Class type of the processor
            //! @param type Type of the processor
            //! @param dataType Datatype of the processor
            //! @param processorPath Path of the processor
            //! @return T* BaseProcessor with path
            //!
            template<class T>
            T* GetChildProcessor(std::string processorPath, BaseModule::ModuleType type = ModuleType::eUndefined, BaseModule::ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                Logger::trace("BaseProcessor::GetChildProcessor(" + processorPath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
                return dynamic_cast<T*>(GetChild(processorPath, type, dataType));
            }
            //!
            //! @brief Get a processor by path
            //!
            //! @tparam T Class type of the processor
            //! @param type Type of the processor
            //! @param dataType Datatype of the processor
            //! @param processorPath Path of the processor
            //! @return T* BaseProcessor with path
            //!
            template<class T>
            static T* GetProcessor(std::string processorPath, BaseModule::ModuleType type = ModuleType::eUndefined, BaseModule::ModuleDataType dataType = ModuleDataType::eUndefined)
            {
                Logger::trace("BaseProcessor::GetProcessor(" + processorPath + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
                // ToDo: return RootProcessor::rootProcessor == nullptr ? nullptr : RootProcessor::rootProcessor->GetChildProcessor<T>(processorPath, type, dataType);
                return nullptr;
            }
    };
} // namespace ModelController