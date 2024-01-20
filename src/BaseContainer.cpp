//!
//! @file BaseContainer.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the BaseContainer
//!
//! @copyright Copyright (c) 2024
//!
#include "BaseContainer.hpp"

namespace ModelController
{
    //!
    //! @brief Construct a new Base Container object
    //!
    BaseContainer::BaseContainer(std::string name, JsonObject config, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
        : BaseModule(name, config, parent, type, dataType)
            {}
} // namespace ModelController
