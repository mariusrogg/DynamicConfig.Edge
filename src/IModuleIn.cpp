//!
//! @file IModuleIn.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the module in interface
//!
//! @copyright Copyright (c) 2023
//!
//!
#include "IModuleIn.hpp"

namespace ModelController
{
    //!
    //! @brief Event raised if module in was created
    //!
    Event<std::string> IModuleIn::ModuleInCreated;
    //!
    //! @brief Wildcard signalizing, that submodules of created modules are ready for connection
    //!
    std::string IModuleIn::wildcardSuffix = "*";
    //!
    //! @brief Construct a new ModuleIn object
    //!
    IModuleIn::IModuleIn(std::string name, BaseModule* parent, ModuleDataType dataType)
        : BaseModule(name, parent, ModuleType::eInput, dataType)
    {
    }
} // namespace ModelController
