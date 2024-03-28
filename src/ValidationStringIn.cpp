//!
//! @file ValidationStringIn.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for validating string input
//!
//! @copyright Copyright (c) 2023
//!
#include "ValidationStringIn.hpp"

namespace ModelController
{
    //!
    //! @brief Call base ctor and initialze members
    //!
    ValidationStringIn::ValidationStringIn(std::string name, std::function<void(std::string)> onInputChanged, std::vector<std::string> defaultValidationValues, BaseModule* parent)
        : ModuleIn<std::string>(name, onInputChanged, parent),
        validationValues("validation", defaultValidationValues, this, true)
    {
    }
    // ToDo: Method to create description of ModuleIn
    //!
    //! @brief Nothing to destruct
    //!
    ValidationStringIn::~ValidationStringIn()
    {
    }
    //!
    //! @brief Collect values by which value is validated and check input value, only sets value, if it matches validation
    //!
    void ValidationStringIn::SetValue(std::string value)
    {
        Logger::trace("ValidationString::SetValue(" + value + ")");
        // ToDo: Performance?
        std::vector<std::string> validationStrings;
        std::vector<std::string> validationRegex;
        for (std::string validationValue : this->validationValues.GetValue())
        {
            // Add values from config (path) to validation
            if (Utils::StartsWith(validationValue, "p:>"))
            {
                std::string path = Utils::TrimStart(validationValue, "p:>", 1);
                JsonVariant config = ConfigFile::GetConfig(GetAbsolutePath(path));
                if (config.is<std::string>())
                {
                    validationStrings.push_back(config.as<std::string>());
                }
                else if (config.is<JsonObject>())
                {
                    for (JsonPair childConfig : config.as<JsonObject>())
                    {
                        validationStrings.push_back(childConfig.key().c_str());
                    }
                }
                else if (config.is<JsonArray>())
                {
                    for (JsonVariant item : config.as<JsonArray>())
                    {
                        if (item.is<std::string>())
                        {
                            validationStrings.push_back(item.as<std::string>());
                        }
                    }
                }
            }
            else if (Utils::StartsWith(validationValue, "r:>"))
            {
                validationRegex.push_back(Utils::TrimStart(validationValue, "r:>", 1));
            }
            else
            {
                validationStrings.push_back(validationValue);
            }
        }
        bool setValue = false;
        if (std::find(validationStrings.begin(), validationStrings.end(), value) != validationStrings.end())
        {
            setValue = true;
        }
        else
        {
            for (std::string regexString : validationRegex)
            {
                std::regex regex(regexString);
                if (std::regex_match(value, regex))
                {
                    setValue = true;
                    break;
                }
            }
        }
        if (setValue)
        {
            ModuleIn<std::string>::SetValue(value);
        }
    }
} // namespace ModelController
