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
    //! @brief Override and do nothing to ensure, that BaseContainer won't contain BaseContainers as children
    //!
    void BaseContainer::SetConfig(JsonObject config)
    {}
    //!
    //! @brief Delete config and object
    //!
    void BaseContainer::Delete()
    {
        ConfigFile::Remove(GetPath());
        delete this;
    }
    //!
    //! @brief Construct a new Base Container object
    //!
    BaseContainer::BaseContainer(std::string name, JsonObject config, BaseModule* parent, BaseModule::ModuleType type, BaseModule::ModuleDataType dataType)
        : BaseModule(name, config, parent, type, dataType)
            {}
} // namespace ModelController
