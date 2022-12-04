#pragma once
#include <ArduinoJson.h>
#include <map>
#include "Device.hpp"
#include "LittleFS.h"

using namespace std;

//!
//! @brief Class for handling hardware
//!
class HardwareHandler
{
private:
    //!
    //! @brief Hardware-Devices
    //!
    std::map<string, Device*> devices;
    //!
    //! @brief Path to config file (json)
    //!
    const char* configFilePath;
    //!
    //! @brief SSID of WIFI-STA
    //!
    string ssid = "LEDController";
    //!
    //! @brief Password of WIFI-STA
    //!
    string wifiPassword = "LEDController";
    //!
    //! @brief Delete all devices and clear map
    //!
    void ClearDevices();

public:
    //!
    //! @brief Default config file path
    //!
    static constexpr const char* defaultConfigFile = "/config.json";
    //!
    //! @brief Construct a new Hardware Handler object
    //!
    //! @param config Configuration of the Hardware
    //!
    HardwareHandler(JsonObject config);
    //!
    //! @brief Construct a new Hardware Handler object
    //!
    //! @param configFilePath Path to configuration file for hardware
    //!
    HardwareHandler(const char* configFilePath = defaultConfigFile);
    //!
    //! @brief Destruction of the Hardware Handler object
    //!
    ~HardwareHandler();
    //!
    //! @brief Get the Device object by name
    //!
    //! @param name Name of the device
    //! @return Device* Device with name
    //!
    Device* GetDevice(string name);
    //!
    //! @brief Update hardware configuration
    //!
    //! @param config New Hardware config
    //!
    void UpdateHardwareConfig(JsonObject config);
    //!
    //! @brief Execute all devices
    //!
    void Execute();
    //!
    //! @brief Update setpoints for devices
    //!
    //! @param config New setpoints for devices
    //!
    void UpdateTargetValues(JsonObject config);
    //!
    //! @brief Update hardware-config/setpoints
    //!
    //! @param config New config
    //!
    void Update(JsonObject config);
    //!
    //! @brief Get the hardware config as string
    //!
    //! @return string Hardware config
    //!
    string GetConfig();
};