#pragma once

#include "WebServer.h"
#include <string>

class API
{
    private:
        static bool listening;
        API() = delete;
        static void handleGetParameters()
        {
            std::string message;
            for (int i = 0; i < server.args(); i++)
            {
                message += server.argName(i).c_str();
                message += "\t=";
                message += server.arg(i).c_str();
                message += "\n";
            }
            server.send(200, "text/plain", message.c_str());
        }
    public:
        static WebServer server;
        static void Initialize()
        {
            server.on("/Parameter", handleGetParameters);
        }
        static void Handle(bool wifiConnected)
        {
            server.handleClient();
            if (wifiConnected)
            {
                if (!listening)
                {
                    API::server.begin();
                    listening = true;
                }
            }
            listening = wifiConnected;
        }
};

WebServer API::server(80);
bool API::listening = false;