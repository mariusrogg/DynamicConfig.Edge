#pragma once
#include "BaseProcessor.hpp"

namespace ModelController
{
    class RootProcessor : public BaseProcessor<RootProcessor>
    {

    public:
        static std::string GetProcessorType()
        {
            return "root";
        }
        //!
        //! @brief Name of the type for config
        //!
        static constexpr const char* type = "root";
        //!
        //! @brief Construct a new Gain object
        //!
        //! @param name Name of the gain
        //! @param config Config
        //! @param parent Parent module
        //!
        static RootProcessor* rootProcessor;
        RootProcessor(std::string name, JsonObject config, BaseModule* parent = nullptr);
    };
} // namespace ModelController
