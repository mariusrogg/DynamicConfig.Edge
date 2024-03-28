//!
//! @file IModuleOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface for output module
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "IModuleConnector.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    class IModuleOut : public IModuleConnector
    {
        public:
            //!
            //! @brief Event raised, if new ModuleOut was created
            //!
            static Event<std::string> ModuleOutCreated;
            //!
            //! @brief Construct a new module out object
            //!
            //! @param name Name of the module
            //! @param parent Parent of the module (normally pass this)
            //! @param dataType DataType of the module
            //!
            IModuleOut(std::string name, BaseModule* parent = nullptr, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
            //!
            //! @brief Set actual value of the input variable
            //!
            //! @param value Value to be set
            //!
            virtual void SetStringValue(std::string value) = 0;
    };
} // namespace ModelController
