#include "Gain.hpp"

namespace ModelController
{
    //!
    //! @brief Multiply input value with gainValue and set value of output
    //!
    void Gain::calculateOut(double value)
    {
        out.SetValue(value * gainValue);
    }
    //!
    //! @brief Construct a new Gain object
    //!
    Gain::Gain(std::string name, JsonObject config, BaseModule* parent)
            : BaseProcessor(name, config, parent, ModuleType::eNone, ModuleDataType::eDouble),
            in("in", config, this),
            out("out", config, this),
            onInputChanged(&(in.ValueChangedEvent), [&](double value) { calculateOut(value); })
        {
            if (config["gain"].is<double>())
            {
                gainValue = config["gain"].as<double>();
            }
        }
} // namespace ModelController
