#include "WiFiHandler.hpp"

unsigned long WiFiHandler::timeout = 5000;
unsigned long WiFiHandler::timeStarted = 0;
int WiFiHandler::reconnects = 0;
int WiFiHandler::reconnectsBeforeFail = 5;
bool WiFiHandler::activeAP = false;

//!
//! @brief Disconnect from WiFi and restart with new settings
//!
void WiFiHandler::BeginSTA(const char* ssid, const char* password)
{
    reconnects = 0;
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    timeStarted = millis();
    Serial.println("Connecting to WiFi ..");
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
    reconnects = 0;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    IPAddress local_IP(192, 168, 0, 90);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress gateway(192, 168, 0, 1);
    if (!WiFi.config(local_IP, gateway, subnet)) 
    {
        Serial.println("STA Failed to configure");
    }
    Serial.println("Starting AP ..");
    return WiFi.softAP(ssid, password);
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
        }
        else if (millis() > timeStarted + timeout)
        {
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
                Serial.println("Try Reconnect");
                retVal = false;
                WiFi.reconnect();
                timeStarted = millis();
            }
        }
        else
        {
            retVal = false;
        }
    }
    else
    {
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

