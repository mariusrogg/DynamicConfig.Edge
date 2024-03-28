//!
//! @file IModuleOut.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the module out interface
//!
//! @copyright Copyright (c) 2023
//!
//!
#include "IModuleOut.hpp"

namespace ModelController
{
    //!
    //! @brief Event raised if module out was created
    //!
    Event<std::string> IModuleOut::ModuleOutCreated;
    //!
    //! @brief Construct a new ModuleOut object
    //!
    IModuleOut::IModuleOut(std::string name, BaseModule* parent, ModuleDataType dataType)
        : IModuleConnector(name, parent, ModuleType::eOutput, dataType)
    {
    }
} // namespace ModelController
