//!
//! @file BaseContainer.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief BaseClass for Processors and Connectors
//!
//! @copyright Copyright (c) 2024
//!
#pragma once
#include "BaseModule.hpp"
#include "ConfigFile.hpp"

namespace ModelController
{
    class BaseContainer : public BaseModule
    {
        protected:
            //!
            //! @brief Set the config to the module
            //!
            //! @param config Config to be set
            //!
            virtual void SetConfig(JsonObject config) override;
            //!
            //! @brief Delete container
            //!
            virtual void Delete() override;
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
