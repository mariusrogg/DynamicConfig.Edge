//!
//! @file Logger.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the logger
//!
//! @copyright Copyright (c) 2023
//!
#include "Logger.hpp"

Logger::Level Logger::minLevel = Logger::Level::eTrace;

//!
//! @brief Swith depending on level and return short name of level
//!
std::string Logger::LevelToString(Level level)
{
    std::string name;
    switch (level)
    {
    case Level::eFatal:
        name = "FATAL";
        break;
    case Level::eError:
        name = "ERROR";
        break;
    case Level::eWarning:
        name = "WARN";
        break;
    case Level::eInfo:
        name = "INFO";
        break;
    case Level::eDebug:
        name = "DEBUG";
        break;
    case Level::eTrace:
        name = "TRACE";
        break;
    default:
        break;
    }
    return name;
}
//!
//! @brief Create log message with timestamp, level and message
//!
void Logger::log(Level level, std::string message, bool logAlways)
{
    if (level <= minLevel || logAlways)
    {
        Serial.print((millis() + "\t" + LevelToString(level) + "\t - ").c_str());
        Serial.println(message.c_str());
    }
}
//!
//! @brief Calls log with trace level
//!
void Logger::trace(std::string message, bool logAlways)
{
    log(Level::eTrace, message, logAlways);
}
//!
//! @brief Calls log with debug level
//!
void Logger::debug(std::string message, bool logAlways)
{
    log(Level::eDebug, message, logAlways);
}
//!
//! @brief Calls log with info level
//!
void Logger::info(std::string message, bool logAlways)
{
    log(Level::eInfo, message, logAlways);
}
//!
//! @brief Calls log with warning level
//!
void Logger::warning(std::string message, bool logAlways)
{
    log(Level::eError, message, logAlways);
}
//!
//! @brief Calls log with error level
//!
void Logger::error(std::string message, bool logAlways)
{
    log(Level::eError, message, logAlways);
}
//!
//! @brief Calls log with fatal level
//!
void Logger::fatal(std::string message, bool logAlways)
{
    log(Level::eFatal, message, logAlways);
}