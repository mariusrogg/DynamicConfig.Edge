//!
//! @file ConfigAPI.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the ConfigAPI
//!
//! @copyright Copyright (c) 2024
//!
#include "ConfigAPI.hpp"
#include "Logger.hpp"
#include "BaseContainer.hpp"
#include "IModuleIn.hpp"
#include "IModuleOut.hpp"

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
        return GetFromArgs("path");
    }
    //!
    //! @brief Iterate through args until arg with name path was found
    //!
    std::string ConfigAPI::GetFromArgs(std::string arg)
    {
        return server.arg(arg.c_str()).c_str();
    }
    //!
    //! @brief Send parameters on specified path
    //!
    void ConfigAPI::handleGetParameters()
    {
        std::string path = GetPathFromArgs();
        Logger::debug("ConfigAPI: Received GetParameters for path " + path);
        std::string message;
        message = ModelController::ConfigFile::GetConfig(path).as<std::string>();
        // ToDo: Maybe handle this later via BaseModule::GetParameters (or similary)
        server.send(200, "text/json", message.c_str());
    }
    //!
    //! @brief Send parameters on specified path
    //!
    void ConfigAPI::handleGetContainers()
    {
        std::string path = GetPathFromArgs();
        std::string type = GetFromArgs("type");
        std::string apiRequested = GetFromArgs("api");
        bool api = Utils::ToBool(apiRequested);
        Logger::debug("ConfigAPI: Received GetContainers for path " + path + " with type '" + type + "' and api '" + apiRequested + "'");
        std::vector<BaseModule*> containers = BaseModule::GetContainers(path, type);
        JsonDocument doc;
        JsonArray arr = doc.to<JsonArray>();
        for (BaseModule* module : containers)
        {
            if (BaseContainer* container = dynamic_cast<BaseContainer*>(module))
            {
                JsonObject obj = arr.add<JsonObject>();
                obj["path"] = container->GetPath();
                obj["type"] = container->GetContainerType();
                if (api)
                {
                    JsonArray apiVars = obj["API"].to<JsonArray>();
                    for (BaseModule* child : container->GetChildren())
                    {
                        bool apiVariable = false;
                        if (IModuleConnector* connector = dynamic_cast<IModuleConnector*>(child) && connector->IsAPIConnected())
                        {
                            JsonObject jVar = apiVars.add<JsonObject>();
                            connector->BuildConfig(jVar);
                        }
                    }
                }
            }
        }

        std::string message = doc.as<std::string>();
        server.send(200, "text/json", message.c_str());
    }
    //!
    //! @brief Delete module on specified path
    //!
    void ConfigAPI::handleDelete()
    {
        std::string path = GetPathFromArgs();
        Logger::debug("ConfigAPI: Received Delete for path " + path);
        BaseModule::Delete(path);
        server.send(200, "text/plain");
    }
    //!
    //! @brief Set module on specified path
    //!
    void ConfigAPI::handleSet()
    {
        std::string path = GetPathFromArgs();
        std::string content = server.arg("plain").c_str();
        Logger::debug("ConfigAPI: Received Set for path " + path + " with content\n" + content);
        std::string message = BaseModule::Set(path, content).c_str();
        if (message.empty())
        {
            server.send(201);
        }
        else
        {
            server.send(400, "text/plain", message.c_str());
        }
    }
    //!
    //! @brief Initialize routes
    //!
    void ConfigAPI::Initialize()
    {
        server.on("/Parameter", handleGetParameters);
        server.on("/Containers", handleGetContainers);
        server.on("/Delete", HTTP_POST, handleDelete);
        server.on("/Set", HTTP_POST, handleSet);
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
