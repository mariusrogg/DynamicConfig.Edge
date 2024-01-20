//!
//! @file ConfigAPI.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief API for getting and updating configuration
//!
//! @copyright Copyright (c) 2024
//!
#pragma once

#include "WebServer.h"
#include <string>
#include "ConfigFile.hpp"

namespace ModelController
{
    class ConfigAPI
    {
        private:
            //!
            //! @brief Webserver for API calls
            //!
            static WebServer server;
            //!
            //! @brief True if API is listening
            //!
            static bool listening;
            //!
            //! @brief True if server was initialized
            //!
            static bool initialized;
            //!
            //! @brief Delete default ctor for pure static object
            //!
            ConfigAPI() = delete;
            //!
            //! @brief Handle method to get parameters
            //!
            static void handleGetParameters();
            //!
            //! @brief Initialize routes
            //!
            static void Initialize();
        public:
            //!
            //! @brief Handle webserver depending on wifi connection state
            //!
            //! @param wifiConnected
            //!
            static void Handle(bool wifiConnected);
    };
}