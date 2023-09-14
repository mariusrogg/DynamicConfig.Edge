#pragma once
#include "WiFi.h"
#include "ArduinoJson.h"
//!
//! @brief Class for connecting to WiFi
//!
class WiFiHandler
{
private:
    //!
    //! @brief Time where WiFi connection was started
    //!
    static unsigned long timeStarted;
    //!
    //! @brief Not used (class is static)
    //!
    WiFiHandler();
    //!
    //! @brief Not used (class is static)
    //!
    ~WiFiHandler();
    //!
    //! @brief Timeout if WiFi is not connected anymore
    //!
    static unsigned long timeout;
    //!
    //! @brief Number of reconnects before AP is started
    //!
    static int reconnectsBeforeFail;
    //!
    //! @brief Number of tried reconnects
    //!
    static int reconnects;
    //!
    //! @brief True if AP is active
    //!
    static bool activeAP;
    //!
    //! @brief SSID of the wifi station
    //!
    static std::string ssid;
    //!
    //! @brief WiFi-Password
    //!
    static std::string password;
    //!
    //! @brief Connect to STA
    //!
    //! @param ssid SSID of the WiFi-Station
    //! @param password Password of the WiFi-Station
    //!
    static void BeginSTA(const char* ssid, const char* password);
    //!
    //! @brief Start AP
    //!
    //! @return true AP successfully started
    //! @return false AP not started
    //!
    static bool BeginAP();
    //!
    //! @brief Start AP
    //!
    //! @param ssid SSID of the AP
    //! @param password Password of the AP
    //!
    //! @return true AP successfully started
    //! @return false AP not started
    //!
    static bool BeginAP(const char* ssid, const char* password);

public:
    //!
    //! @brief Check if WiFi is connected
    //!
    //! @return true WiFi is connected
    //! @return false WiFi is not connected
    //!
    static bool Check();
    //!
    //! @brief Set ssid and password for WiFi-Connection
    //!
    //! @param ssid SSID of the WiFi-Station
    //! @param password Password of the WiFi-Station
    //!
    static void SetSSIDPassword(std::string ssid, std::string password);
    //!
    //! @brief Get ssid
    //!
    //! @return std::string SSID of the WiFi-Station
    //!
    static std::string GetSSID();
    //!
    //! @brief Get Password
    //!
    //! @return std::string Password of the WiFi-Station
    //!
    static std::string GetPassword();
};