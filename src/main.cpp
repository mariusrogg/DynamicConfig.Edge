#include <Arduino.h>
#include "LittleFS.h"
#include <string>
#include <ArduinoJson.h>
#include <vector>
#include "WiFi.h"
#include "HardwareHandler.hpp"
#include "WiFiHandler.hpp"
#include <WebServer.h>

using namespace std;

//!
//! @brief Calculating setpoints for hardware components
//!
HardwareHandler* hwHandler = nullptr;

WebServer server(80);

void handleGetConfig() 
{
    server.send(200, "text/json", hwHandler->GetConfig().c_str());
}
void handlePostConfig() 
{
    if (server.hasArg("plain"))
    {
        string body = server.arg("plain").c_str();
        DynamicJsonDocument jsonDocument(32768);
        deserializeJson(jsonDocument, body);
        hwHandler->UpdateHardwareConfig(jsonDocument.as<JsonObject>());
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(204, "text/plain", "No content");
    }
}
void handlePostTarget() 
{
    if (server.hasArg("plain"))
    {
        string body = server.arg("plain").c_str();
        DynamicJsonDocument jsonDocument(32768);
        deserializeJson(jsonDocument, body);
        hwHandler->UpdateTargetValues(jsonDocument.as<JsonObject>());
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(204, "text/plain", "No content");
    }
}
void handleNotFound()
{
    server.send(404, "text/plain", "Not Found");
}

//!
//! @brief Handle request message sent to server
//!
//! Updates config/setpoints of Hardware Handler
//!
//! @param input Message sent to server
//!
void HandleServerRequest(string input)
{
    char* inputAsChar = (char*)input.c_str();
    DynamicJsonDocument config(32768);
    deserializeJson(config, inputAsChar);
    if (input == "config")
    {
    }
    else
    {
        hwHandler->Update(config.as<JsonObject>());
    }
}
void setup()
{
    // Initialize serial console
    Serial.begin(115200);
    Serial.println("\n\n");
    Serial.println("started");

    server.on("/config", handleGetConfig);
    server.on("/config", HTTP_POST, handlePostConfig);
    server.on("/target", HTTP_POST, handlePostTarget);
    server.onNotFound(handleNotFound);

    // Check if Filesystem was initialized
    if (!LittleFS.begin(true, ""))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }
    hwHandler = new HardwareHandler();
    Serial.println(hwHandler->GetConfig().c_str());
}

bool listening = false;

void loop()
{
    server.handleClient();
    // Calculate setpoints for Hardware
    hwHandler->Execute();
    // Check if WiFi is connected
    if (WiFiHandler::Check())
    {
        if (!listening)
        {
            // Start TCP Server for Communication
            Serial.println(WiFi.localIP());
            server.begin();
            // Server.begin();
            listening = true;
        }
    }
}