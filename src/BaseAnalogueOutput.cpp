//!
//! @file BaseAnalogueOutput.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the analog output base class
//! @version 0.0.0
//! @date 2023-09-14
//!
//! @copyright Copyright (c) 2023
//!
#include "BaseAnalogueOutput.hpp"

namespace ModelController
{
    //!
    //! @brief Construct a new Base Analogue Output object
    //!
    BaseAnalogueOutput::BaseAnalogueOutput(std::string name, JsonObject config, Connector* parent)
        : BaseOutputConnector(name, config, parent)
    {
    }
} // namespace ModelController
