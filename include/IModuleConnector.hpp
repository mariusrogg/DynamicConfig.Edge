#pragma once
#include "BaseModule.hpp"

namespace ModelController
{
    class IModuleConnector : public BaseModule
    {
        public:
            //!
            //! @brief Wildcard showing, that all ouput submodules are available for inputs
            //!
            static std::string wildcardSuffix;
            //!
            //! @brief Check if variable is connected to API
            //!
            //! @return true If connected to API
            //! @return false If connected to output
            //!
            virtual bool IsAPIConnected() const = 0;
            IModuleConnector(std::string name, BaseModule* parent = nullptr, ModuleType type = ModuleType::eUndefined, ModuleDataType dataType = ModuleDataType::eUndefined);
            virtual void BuildConfig(JsonObject config);
    };

} // namespace ModelController
