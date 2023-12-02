//!
//! @file Utils.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Helper functions
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <string>

namespace ModelController
{
    class Utils
    {
        private:
            //!
            //! @brief Pure static class -> deleted ctor
            //!
            Utils() = delete;
        public:
            //!
            //! @brief Trim (remove) all occurences of a string at the beginning of another string
            //!
            //! @param origin String to remove from
            //! @param trim String to be removed
            //! @return std::string Trimmed string
            //!
            static std::string TrimStart(std::string origin, std::string trim);
            //!
            //! @brief Trim (remove) all occurences of a string at the end of another string
            //!
            //! @param origin String to remove from
            //! @param trim String to be removed
            //! @return std::string Trimmed string
            //!
            static std::string TrimEnd(std::string origin, std::string trim);
            //!
            //! @brief Trim (remove) all occurences of a string at the beginnging and end of another string
            //!
            //! @param origin String to remove from
            //! @param trim String to be removed
            //! @return std::string Trimmed string
            //!
            static std::string Trim(std::string origin, std::string trim);
    };
} // namespace ModelController
