//!
//! @file BaseProcessor.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the processor
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseProcessor.hpp"
#include "Logger.hpp"
#include "Gain.hpp"
#include "SequenceProcessor.hpp"
#include "RootProcessor.hpp"

namespace ModelController
{
    //!
    //! @brief Construct a new Processor object
    //!
    template<typename Derived>
    BaseProcessor<Derived>::BaseProcessor(std::string name, JsonObject config, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
        : BaseProcessor(name, parent, config["shortPath"].is<bool>() ? config["shortPath"].as<bool>() : false, type, dataType)
    {
        Logger::trace("BaseProcessor::BaseProcessor(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        ConfigFile::SetConfig(GetPath() + "/type", Derived::GetProcessorType());
        for (JsonPair child : config)
        {
            if (child.value().is<JsonObject>())
            {
                Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                GenerateProcessor(child.key().c_str(), child.value(), this);
            }
        }
    }
    //!
    //! @brief Construct a new Processor object
    //!
    template<typename Derived>
    BaseProcessor<Derived>::BaseProcessor(std::string name, BaseModule* parent, bool createShortPath, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
    {
        Logger::trace("BaseProcessor::BaseProcessor(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Initialize(name, parent, createShortPath, type, dataType);
    }
    //!
    //! @brief Destruction of the Base Base Processor object
    //!
    template<typename Derived>
    BaseProcessor<Derived>::~BaseProcessor()
    {
        Logger::trace("BaseProcessor::~BaseProcessor() - " + GetPath());
    }
    //!
    //! @brief Generates processor from json
    //!
    template<typename Derived>
    IBaseProcessor* BaseProcessor<Derived>::GenerateProcessor(std::string name, JsonObject processorConfig, IBaseProcessor* parent)
    {
        Logger::trace("BaseProcessor::GenerateProcessor(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ")");
        IBaseProcessor* processor = nullptr;
        if (processorConfig["type"].is<std::string>())
        {
            std::string type = processorConfig["type"].as<std::string>();
            if (type == Gain::type)
            {
                processor = new Gain(name, processorConfig, parent);
            }
            else if (type == SequenceProcessor::type)
            {
                processor = new SequenceProcessor(name, processorConfig, parent);
            }
            else
            {
                for (JsonPair child : processorConfig)
                {
                    if (child.value().is<JsonObject>())
                    {
                        Logger::trace("Child found in json: " + std::string(child.key().c_str()));
                        GenerateProcessor(name + "/" + child.key().c_str(), child.value(), parent);
                    }
                }
            }

        }
        return processor;
    }
    template class BaseProcessor<SequenceProcessor>;
    template class BaseProcessor<RootProcessor>;
    template class BaseProcessor<Gain>;
} // namespace ModelController
