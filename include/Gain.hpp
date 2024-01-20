//!
//! @file Gain.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Processor multiplying input value by constant value and putting it to output
//!
//! @copyright Copyright (c) 2023
//!
//!
#pragma once
#include "ModuleOut.hpp"
#include "ModuleIn.hpp"
#include "BaseContainer.hpp"
#include "ConfigItem.hpp"

namespace ModelController
{
    class Gain : public BaseContainer
    {
        private:
            //!
            //! @brief Input value
            //!
            ModuleIn<double> in;
            //!
            //! @brief Output value
            //!
            ModuleOut<double> out;
            //!
            //! @brief Multiplicator
            //!
            ConfigItem<double> gain;
            //!
            //! @brief Calculate output depending on input0
            //!
            //! @param value Input value
            //!
            void calculateOut(double value);

        public:
            //!
            //! @brief Name of the type for config
            //!
            static constexpr const char* type = "gain";
            //!
            //! @brief Construct a new Gain object
            //!
            //! @param name Name of the gain
            //! @param config Config
            //! @param parent Parent module
            //!
            Gain(std::string name, JsonObject config, BaseModule* parent = nullptr);
    };

} // namespace ModelController
