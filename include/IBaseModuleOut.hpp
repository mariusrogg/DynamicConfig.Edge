//!
//! @file IBaseModuleOut.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Interface for
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "BaseModule.hpp"
#include "EventHandling.hpp"

namespace ModelController
{
    class IBaseModuleOut : public virtual BaseModule
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
    };
} // namespace ModelController
