//!
//! @file IBaseModuleIn.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the module in interface
//!
//! @copyright Copyright (c) 2023
//!
//!
#include "IBaseModuleIn.hpp"

namespace ModelController
{
    //!
    //! @brief Event raised if module in was created
    //!
    Event<std::string> IBaseModuleIn::ModuleInCreated;
    //!
    //! @brief Wildcard signalizing, that submodules of created modules are ready for connection
    //!
    std::string IBaseModuleIn::wildcardSuffix = "*";
} // namespace ModelController
