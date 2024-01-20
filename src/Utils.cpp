//!
//! @file Utils.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implmenetation of the utils (helper functions)
//!
//! @copyright Copyright (c) 2023
//!
#include "Utils.hpp"
#include "esp_random.h"

namespace ModelController
{
    //!
    //! @brief Remove trim at beginning of origin as long as it is not found anymore
    //!
    std::string Utils::TrimStart(std::string origin, std::string trim)
    {
        while (StartsWith(origin, trim))
        {
            origin.erase(0, trim.size());
        }
        return origin;
    }
    //!
    //! @brief Remove trim at end of origin as long as it is not found anymore
    //!
    std::string Utils::TrimEnd(std::string origin, std::string trim)
    {
        while (EndsWith(origin, trim))
        {
            origin.erase(origin.size() - trim.size(), trim.size());
        }
        return origin;
    }
    //!
    //! @brief Call TrimStart and TrimEnd
    //!
    std::string Utils::Trim(std::string origin, std::string trim)
    {
        origin = TrimStart(origin, trim);
        origin = TrimEnd(origin, trim);
        return origin;
    }
    //!
    //! @brief Check if value starts with check
    //!
    bool Utils::StartsWith(std::string value, std::string check)
    {
        return value.find(check.c_str()) == 0;
    }
    //!
    //! @brief Check if value ends with check
    //!
    bool Utils::EndsWith(std::string value, std::string check)
    {
        return value.size() >= check.size() ? value.find(check.c_str(), value.size() - check.size()) == value.size() - check.size() : false;
    }
    //!
    //! @brief Returns randum number with specified length
    //!
    std::string Utils::GetRandomNumber(int length)
    {
        std::string random = std::to_string(esp_random());
        if (random.size() >= length)
        {
            random = random.substr(0, length);
        }
        return random;
    }

} // namespace ModelController
