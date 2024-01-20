//!
//! @file ConfigAPI.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the ConfigAPI
//!
//! @copyright Copyright (c) 2024
//!
#include "ConfigAPI.hpp"

namespace ModelController
{
    //!
    //! @brief Webserver for API calls
    //!
    WebServer ConfigAPI::server(80);
    //!
    //! @brief True if API is listening
    //!
    bool ConfigAPI::listening = false;
    //!
    //! @brief True if server was initialized
    //!
    bool ConfigAPI::initialized = false;
    //!
    //! @brief Send parameters on specified path
    //!
    void ConfigAPI::handleGetParameters()
    {
        std::string message;
        std::string path = "";
        for (int i = 0; i < server.args(); i++)
        {
            if (server.argName(i) == "path")
            {
                path = server.arg(i).c_str();
            }
        }
        message = ModelController::ConfigFile::GetConfig(path).as<std::string>();
        // ToDo: Maybe handle this later via BaseModule::GetParameters (or similary)
        server.send(200, "text/plain", message.c_str());
    }
    //!
    //! @brief Initialize routes
    //!
    void ConfigAPI::Initialize()
    {
        server.on("/Parameter", handleGetParameters);
    }
    //!
    //! @brief Initialize server if not done yet, handle client requests and start server if not done yet or wifi disconnected
    //!
    void ConfigAPI::Handle(bool wifiConnected)
    {
        if (!initialized)
        {
            Initialize();
            initialized = true;
        }
        server.handleClient();
        if (wifiConnected)
        {
            if (!listening)
            {
                ConfigAPI::server.begin();
                listening = true;
            }
        }
        listening = wifiConnected;
    }
} // namespace ModelController
