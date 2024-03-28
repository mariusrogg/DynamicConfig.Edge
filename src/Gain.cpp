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
        out.SetValue(value * gain);
    }
    //!
    //! @brief Construct a new Gain object
    //!
    Gain::Gain(std::string name, BaseModule* parent)
            : BaseContainer(name, parent, ModuleType::eNone, ModuleDataType::eDouble),
            in("in", [&](double value) { calculateOut(value); }, this),
            out("out", this),
            gain("gain", 1, this)
        {
        }
    //!
    //! @brief Returns type
    //!
    std::string Gain::GetContainerType()
    {
        return type;
    }
} // namespace ModelController
