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
    //! @brief Iterate through args until arg with name path was found
    //!
    std::string ConfigAPI::GetPathFromArgs()
    {
        std::string path = "";
        for (int i = 0; i < server.args() && path.empty(); i++)
        {
            if (server.argName(i) == "path")
            {
                path = server.arg(i).c_str();
            }
        }
        return path;
    }
    //!
    //! @brief Send parameters on specified path
    //!
    void ConfigAPI::handleGetParameters()
    {
        std::string message;
        message = ModelController::ConfigFile::GetConfig(GetPathFromArgs()).as<std::string>();
        // ToDo: Maybe handle this later via BaseModule::GetParameters (or similary)
        server.send(200, "text/json", message.c_str());
    }
    //!
    //! @brief Delete module on specified path
    //!
    void ConfigAPI::handleDelete()
    {
        BaseModule::Delete(GetPathFromArgs());
        server.send(200, "text/plain");
    }
    //!
    //! @brief Initialize routes
    //!
    void ConfigAPI::Initialize()
    {
        server.on("/Parameter", handleGetParameters);
        server.on("/Delete", HTTP_POST, handleDelete);
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
