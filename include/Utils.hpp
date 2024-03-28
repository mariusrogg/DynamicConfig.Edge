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
#include <ArduinoJson.h>
#include <vector>
#include <algorithm>

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
            //! @param repetition Maximum amount of times the string should be deleted (-1 for infinite)
            //! @return std::string Trimmed string
            //!
            static std::string TrimStart(std::string origin, std::string trim, int repetition = -1);
            //!
            //! @brief Trim (remove) all occurences of a string at the end of another string
            //!
            //! @param origin String to remove from
            //! @param trim String to be removed
            //! @param repetition Maximum amount of times the string should be deleted (-1 for infinite)
            //! @return std::string Trimmed string
            //!
            static std::string TrimEnd(std::string origin, std::string trim, int repetition = -1);
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
            //!
            //! @brief Create string containing random number
            //!
            //! @param length Length of the returned string
            //! @return std::string String with random number
            //!
            static std::string GetRandomNumber(int length);
            //!
            //! @brief Convert json array to vector
            //!
            //! @tparam T Type of vector elements
            //! @param jsonArray Array to be converted
            //! @return std::vector<T> Converted array
             //!
            template<typename T>
            static std::vector<T> JsonArrayToArray(JsonArray jsonArray)
            {
                std::vector<T> array;
                for (JsonVariant item : jsonArray)
                {
                    if (item.is<T>())
                    {
                        array.push_back(item.as<T>());
                    }
                }
                return array;
            }
            //!
            //! @brief Convert array to JsonArray
            //!
            //! @tparam T Encapsulated array type
            //! @param array Array to be converted
            //! @param jsonArray Array to write valeus to
            //! @param clear True if JsonArray is cleared before converting
             //!
            template<typename T>
            static void ArrayToJsonArray(std::vector<T> array, JsonArray jsonArray, bool clear = true)
            {
                if (clear)
                {
                    jsonArray.clear();
                }
                for (T item : array)
                {
                    jsonArray.add(item);
                }
            //! @brief Tranform string value to bool
            //!
            //! @param value Value to be transformed
            //! @return false Value represents false ("0", "false", "n", "no")
            //! @return true All other values
            //!
            static bool ToBool(std::string value)
            {
                std::transform(value.begin(), value.end(), value.begin(), tolower);
                return value != "0" && value != "false" && value != "n" && value != "no";
            }
    };
} // namespace ModelController
