//!
//! @file IModuleOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface for output module
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    class IModuleOut : public BaseModule
    {
        public:
            //!
            //! @brief Wildcard showing, that all ouput submodules are available for inputs
            //!
            static std::string wildcardSuffix;
            //!
            //! @brief Event raised, if new ModuleOut was created
            //!
            static Event<std::string> ModuleOutCreated;
            //!
            //! @brief Check if variable is connected to API
            //!
            //! @return true If connected to API
            //! @return false If connected to output
            //!
            virtual bool IsAPIConnected() const = 0;
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
