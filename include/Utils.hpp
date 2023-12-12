//!
//! @file Utils.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Helper functions
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <string>
#include <sstream>

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
            //!
            //! @brief Convert value to string
            //!
            //! @tparam T Type of the value to be converted
            //! @param value Value to be converted
            //! @return std::string String, generated out of value
            //!
            template<typename T>
            static std::string ToString(T value)
            {
                std::ostringstream strVal;
                strVal << value;
                return strVal.str();
            }
            //!
            //! @brief Convert string to value
            //!
            //! @tparam T Type to convert string to
            //! @param value Value to be converted
            //! @return T Value, generated out of string
            //!
            template<typename T>
            static T FromString(std::string value)
            {
                std::stringstream convert(value);
                T val;
                convert >> val;
                return val;
            }
            //!
            //! @brief Check if string (value) starts with other string (check)
            //!
            //! @param value String to be checked
            //! @param check String to compare
            //! @return true Value starts with check
            //! @return false Value does not start with check
            //!
            static bool StartsWith(std::string value, std::string check);
            //!
            //! @brief Check if string (value) ends with other string (check)
            //!
            //! @param value String to be checked
            //! @param check String to compare
            //! @return true Value ends with check
            //! @return false Value does not end with check
            //!
            static bool EndsWith(std::string value, std::string check);
    };
} // namespace ModelController
