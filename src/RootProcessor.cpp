#include "RootProcessor.hpp"

namespace ModelController
{
    RootProcessor::RootProcessor(std::string name, JsonObject config, BaseModule* parent)
        : BaseProcessor<RootProcessor>(name, config, parent)
    {
    }
    RootProcessor* RootProcessor::rootProcessor = nullptr;
} // namespace ModelController
