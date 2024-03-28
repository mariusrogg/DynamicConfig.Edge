//!
//! @file IModuleIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface for input module
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "IModuleConnector.hpp"
#include "EventHandling.hpp"

namespace ModelController
{ // ToDo: inform out on deletion and other way round
    class IModuleIn : public IModuleConnector
    {
        public:
            //!
            //! @brief Event raised, if new ModuleIn was created
            //!
            static Event<std::string> ModuleInCreated;
            //!
            //! @brief Construct a new module in object
            //!
            //! @param name Name of the module
            //! @param parent Parent of the module (normally pass this)
            //! @param dataType DataType of the module
            //!
            IModuleIn(std::string name, BaseModule* parent = nullptr, BaseModule::ModuleDataType dataType = BaseModule::ModuleDataType::eUndefined);
    };
} // namespace ModelController
