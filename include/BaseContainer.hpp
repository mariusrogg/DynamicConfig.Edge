//!
//! @file BaseContainer.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief BaseClass for Processors and Connectors
//!
//! @copyright Copyright (c) 2024
//!
#pragma once
#include "BaseModule.hpp"

namespace ModelController
{
    class BaseContainer : public BaseModule
    {
    public:
        //!
        //! @brief Construct a new Base Container object
        //!
        //! @param name Name of the module
        //! @param config Config of the module
        //! @param parent Parent of the module (normally pass this)
        //! @param type Type of the module
        //! @param dataType DataType of the module
        //!
        BaseContainer(std::string name, JsonObject config, BaseModule* parent = nullptr, BaseModule::ModuleType type = BaseModule::ModuleType::eUndefined, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
    };
} // namespace ModelController
