//!
//! @file WiFiHandler.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the WiFi-Handler
//!
//! @copyright Copyright (c) 2023
//!
#include "WiFiHandler.hpp"
#include "Logger.hpp"

unsigned long WiFiHandler::timeout = 5000;
unsigned long WiFiHandler::timeStarted = 0;
int WiFiHandler::reconnects = 0;
int WiFiHandler::reconnectsBeforeFail = 5;
bool WiFiHandler::activeAP = false;
std::string WiFiHandler::ssid = "Controller";
std::string WiFiHandler::password = "Controller";
bool WiFiHandler::staConnected = false;
ModelController::Event<> WiFiHandler::STAConnected;
ModelController::Event<> WiFiHandler::APInitialized;

//!
//! @brief Disconnect from WiFi and restart with new settings
//!
void WiFiHandler::BeginSTA(const char* ssid, const char* password)
{
    reconnects = 0;
    staConnected = false;
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    timeStarted = millis();
    Logger::debug("WiFi: Connecting to WiFi");
}
//!
//! @brief Start AP with default values
//!
bool WiFiHandler::BeginAP()
{
    return BeginAP("LEDController", "LEDController");
}
//!
//! @brief Disconnect WiFi and start AP
//!
bool WiFiHandler::BeginAP(const char* ssid, const char* password)
{
    staConnected = false;
    bool success = false;
    reconnects = 0;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    IPAddress local_IP(192, 168, 0, 90);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress gateway(192, 168, 0, 1);
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Logger::error("WiFi: STA Failed to configure");
    }
    Logger::info("WiFi: Starting AP");
    if (WiFi.softAP(ssid, password))
    {
        APInitialized.Raise();
        success = true;
    }
    return success;
}
//!
//! @brief Check if WiFi is connected
//!
bool WiFiHandler::Check()
{
    bool retVal = false;
    //!
    //! @brief Check WiFi mode
    //!
    if (WiFi.getMode() == WIFI_STA)
    {
        //!
        //! @brief Return true if WiFi-Station is connected
        //!
        if (WiFi.status() == WL_CONNECTED)
        {
            timeStarted = millis();
            retVal = true;
            reconnects = 0;
            if (!staConnected)
            {
                Logger::info("WiFi: STA Connected");
                STAConnected.Raise();
                staConnected = true;
            }
        }
        else if (millis() > timeStarted + timeout)
        {
            staConnected = false;
            //!
            //! @brief Reconnect if WiFi-Station is not connected
            //!
            if (++reconnects > reconnectsBeforeFail)
            {
                retVal = activeAP = BeginAP();
            }
            //!
            //! @brief Start STA if WiFi connection could not be restarted
            //!
            else
            {
                Logger::warning("WiFi: Try Reconnect");
                retVal = false;
                WiFi.reconnect();
                timeStarted = millis();
            }
        }
        else
        {
            staConnected = false;
            retVal = false;
        }
    }
    else
    {
        staConnected = false;
        //!
        //! @brief Return true if AP is active/started successfully
        //!
        if (activeAP)
        {
            retVal = true;
        }
        else
        {
            retVal = activeAP = BeginAP();
        }
    }
    return retVal;
}
//!
//! @brief Set ssid and password and begin STA, if they changed
//!
void WiFiHandler::SetSSIDPassword(std::string ssid, std::string password)
{
    if (WiFiHandler::ssid != ssid || WiFiHandler::password != password)
    {
        WiFiHandler::ssid = ssid;
        WiFiHandler::password = password;
        BeginSTA(ssid.c_str(), password.c_str());
    }
    else if (WiFi.getMode() != WIFI_STA)
    {
        BeginSTA(ssid.c_str(), password.c_str());
    }
}
//!
//! @brief Returns WiFi-SSID
//!
std::string WiFiHandler::GetSSID()
{
    return ssid;
}
//!
//! @brief Returns WiFi-Password
//!
std::string WiFiHandler::GetPassword()
{
    return password;
}

