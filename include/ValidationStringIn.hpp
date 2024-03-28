//!
//! @file ValidationStringIn.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for validating string input
//!
//! @copyright Copyright (c) 2023
//!
#pragma once

#include "ModuleIn.hpp"
#include "ConfigArray.hpp"
#include <algorithm>
#include <regex>

namespace ModelController
{
    class ValidationStringIn : public ModuleIn<std::string>
    {
        private:
            //!
            //! @brief Values to validate input
            //!
            ConfigArray<std::string> validationValues;
        public:
            //!
            //! @brief Construct a new Validation String In object
            //!
            //! @param name Name of the connector
            //! @param onInputChanged Function called, if input changed
            //! @param parent Parent of the Connector (normally pass this)
            //!
            ValidationStringIn(std::string name, std::function<void(std::string)> onInputChanged, std::vector<std::string> defaultValidationValues = {""}, BaseModule* parent = nullptr);
            //!
            //! @brief Destruction of the Validation String In object
            //!
            ~ValidationStringIn();
            //!
            //! @brief Set the value to the input variable
            //!
            //! @param value Value set to the input variable
             //!
            virtual void SetValue(std::string value) override;
            virtual std::vector<std::string> GetValidationStrings();
            virtual std::vector<std::string> GetValidationRegex();
            virtual void BuildConfig(JsonObject config) override;
    };
} // namespace ModelController
