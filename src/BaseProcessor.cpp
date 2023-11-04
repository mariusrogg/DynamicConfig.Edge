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

namespace ModelController
{
    //!
    //! @brief Root processor
    //!
    BaseProcessor* BaseProcessor::rootProcessor = nullptr;
    //!
    //! @brief Construct a new Processor object
    //!
    BaseProcessor::BaseProcessor(std::string name, JsonObject config, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
        : BaseProcessor(name, parent, type, dataType)
    {
        Logger::trace("BaseProcessor::BaseProcessor(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
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
    BaseProcessor::BaseProcessor(std::string name, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
    {
        Logger::trace("BaseProcessor::BaseProcessor(" + name + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ", " + TypeToString(type) + ", " + DataTypeToString(dataType) + ")");
        Initialize(name, parent, type, dataType);
    }
    //!
    //! @brief Destruction of the Base Base Processor object
    //!
    BaseProcessor::~BaseProcessor()
    {
        Logger::trace("BaseProcessor::~BaseProcessor() - " + GetPath());
    }
    //!
    //! @brief Generates processor from json
    //!
    BaseProcessor* BaseProcessor::GenerateProcessor(std::string name, JsonObject processorConfig, BaseProcessor* parent)
    {
        Logger::trace("BaseProcessor::GenerateProcessor(" + name + ", " + "json-config" + ", " + (parent == nullptr ? "NULL" : parent->GetPath()) + ")");
        BaseProcessor* processor = nullptr;
        if (processorConfig["type"].is<std::string>())
        {
            std::string type = processorConfig["type"].as<std::string>();
            if (type == "gain")
            {
                processor = new Gain(name, processorConfig, parent);
            }
        }
        return processor;
    }
} // namespace ModelController
