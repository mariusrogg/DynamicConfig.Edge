//!
//! @file Gain.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Processor multiplying input value by constant value and putting it to output
//!
//! @copyright Copyright (c) 2023
//!
//!
#pragma once
#include "ProcessorOut.hpp"
#include "ProcessorIn.hpp"
#include "BaseProcessor.hpp"

namespace ModelController
{
    class Gain : public BaseProcessor
    {
    private:
        //!
        //! @brief Input value
        //!
        ProcessorIn<double> in;
        //!
        //! @brief Output value
        //!
        ProcessorOut<double> out;
        //!
        //! @brief Multiplicator
        //!
        double gainValue = 1;
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
