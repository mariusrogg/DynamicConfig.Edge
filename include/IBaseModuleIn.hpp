//!
//! @file IBaseModuleIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface for input module
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    class IBaseModuleIn : public virtual BaseModule
    {
        public:
            //!
            //! @brief Wildcard showing, that all ouput submodules are available for inputs
            //!
            static std::string wildcardSuffix;
            //!
            //! @brief Event raised, if new ModuleIn was created
            //!
            static Event<std::string> ModuleInCreated;
    };
} // namespace ModelController
