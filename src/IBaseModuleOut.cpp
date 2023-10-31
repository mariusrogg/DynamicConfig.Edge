//!
//! @file IBaseModuleOut.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the module out interface
//!
//! @copyright Copyright (c) 2023
//!
//!
#include "IBaseModuleOut.hpp"

namespace ModelController
{
    //!
    //! @brief Event raised if module out was created
    //!
    Event<std::string> IBaseModuleOut::ModuleOutCreated;
    //!
    //! @brief Wildcard signalizing, that submodules of created modules are ready for connection
    //!
    std::string IBaseModuleOut::wildcardSuffix = "*";
} // namespace ModelController
