//!
//! @file Gain.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the gain
//!
//! @copyright Copyright (c) 2023
//!
#include "Gain.hpp"

namespace ModelController
{
    //!
    //! @brief Multiply input value with gainValue and set value of output
    //!
    void Gain::calculateOut(double value)
    {
        out.SetValue(value * gain.GetValue());
    }
    //!
    //! @brief Construct a new Gain object
    //!
    Gain::Gain(std::string name, JsonObject config, BaseModule* parent)
            : BaseProcessor(name, config, parent, ModuleType::eNone, ModuleDataType::eDouble),
            in("in", config, [&](double value) { calculateOut(value); }, this),
            out("out", config, this),
            gain("gain", config, 1, this)
        {
        }
} // namespace ModelController
