//!
//! @file Logger.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for Logging errors, warnings, etc.
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <string>
#include <Arduino.h>

class Logger
{
    public:
        //!
        //! @brief Log levels
        //!
        enum class Level
        {
            eFatal,
            eError,
            eWarning,
            eInfo,
            eDebug,
            eTrace,
        };
    private:
        //!
        //! @brief Minimum level beeing logged by the logger
        //!
        static Level minLevel;
        Logger() = delete;
    public:
        //!
        //! @brief Convert log level into readable string
        //!
        //! @param level Level to be converted
        //! @return std::string (Short) name of the level
        //!
        static std::string LevelToString(Level level);
        //!
        //! @brief Create log message
        //!
        //! @param level Level of the message
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void log(Level level, std::string message, bool logAlways = false);
        //!
        //! @brief Log message with trace level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void trace(std::string message, bool logAlways = false);
        //!
        //! @brief Log message with debug level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void debug(std::string message, bool logAlways = false);
        //!
        //! @brief Log message with info level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void info(std::string message, bool logAlways = false);
        //!
        //! @brief Log message with warning level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void warning(std::string message, bool logAlways = false);
        //!
        //! @brief Log message with error level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void error(std::string message, bool logAlways = false);
        //!
        //! @brief Log message with fatal level
        //!
        //! @param message Message to be logged
        //! @param logAlways True if message should be logged ignoring minLevel
        //!
        static void fatal(std::string message, bool logAlways = false);
};