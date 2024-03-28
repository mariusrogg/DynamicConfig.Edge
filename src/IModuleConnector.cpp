#include "IModuleConnector.hpp"

namespace ModelController
{
    //!
    //! @brief Wildcard signalizing, that submodules of created modules are ready for connection
    //!
    std::string IModuleConnector::wildcardSuffix = "*";
    //!
    //! @brief Construct a new ModuleOut object
    //!
    IModuleConnector::IModuleConnector(std::string name, BaseModule* parent, ModuleType type, ModuleDataType dataType)
        : BaseModule(name, parent, type, dataType)
    {
    }
    void IModuleConnector::BuildConfig(JsonObject config)
    {
        config["name"] = GetName();
        config["type"] = BaseModule::TypeToString(GetType());
        config["dataType"] = BaseModule::DataTypeToString(GetDataType());
    }
} // namespace ModelController

