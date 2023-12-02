//!
//! @file Utils.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implmenetation of the utils (helper functions)
//!
//! @copyright Copyright (c) 2023
//!
#include "Utils.hpp"

namespace ModelController
{
    //!
    //! @brief Remove trim at beginning of origin as long as it is not found anymore
    //!
    std::string Utils::TrimStart(std::string origin, std::string trim)
    {
        while (origin.find(trim.c_str()) == 0)
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
        if (origin.size() >= trim.size())
        {
            while (origin.find(trim.c_str(), origin.size() - trim.size()) == origin.size() - trim.size())
            {
                origin.erase(origin.size() - trim.size(), trim.size());
            }
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

} // namespace ModelController
