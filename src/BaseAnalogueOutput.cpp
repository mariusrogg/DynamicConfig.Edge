#include "BaseAnalogueOutput.hpp"

namespace ModelController
{
    BaseAnalogueOutput::BaseAnalogueOutput(std::string name, JsonObject config, HWDevice* parent)
        : HWDevice(name, config, parent)
    {
    }

    BaseAnalogueOutput::~BaseAnalogueOutput()
    {
    }
} // namespace ModelController
