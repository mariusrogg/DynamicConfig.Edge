#include "HardwareHandler.hpp"
#include <string>
#include "WiFiHandler.hpp"
#include <sstream>
#include "DevicePWM.hpp"

//!
//! @brief Delete all devices and clear map
//!
void HardwareHandler::ClearDevices()
{
    for (std::map<string, Device*>::iterator it = devices.begin(); it != devices.end(); ++it)
    {
        if (it->second != nullptr)
        {
            delete it->second;
        }
    }
    devices.clear();
}
//!
//! @brief Update hardware config by given config
//!
HardwareHandler::HardwareHandler(JsonObject config)
{
    UpdateHardwareConfig(config);
}
//!
//! @brief Update hardware config by content of file
//!
HardwareHandler::HardwareHandler(const char* configFilePath)
    : configFilePath(configFilePath)
{
    File file = LittleFS.open(configFilePath);
    DynamicJsonDocument jsonDoc(32768);

    deserializeJson(jsonDoc, file);
    file.close();
    UpdateHardwareConfig(jsonDoc.as<JsonObject>());
}
//!
//! @brief Delete content of map
//!
HardwareHandler::~HardwareHandler()
{
    ClearDevices();
}
//!
//! @brief Finds device with name
//!
Device* HardwareHandler::GetDevice(string name)
{
    Device* device = nullptr;
    std::map<string, Device*>::iterator pos = devices.find(name);
    if (pos != devices.end())
    {
        device = pos->second;
    }
    return device;
}
//!
//! @brief Update WiFi, devices and config file
//!
void HardwareHandler::UpdateHardwareConfig(JsonObject config)
{
    DynamicJsonDocument configDoc(32768);
    if (config.isNull())
    {
        config = configDoc.to<JsonObject>();
    }
    if (config["wifi"]["ssid"].isNull())
    {
        config["wifi"]["ssid"] = this->ssid;
    }
    if (config["wifi"]["password"].isNull())
    {
        config["wifi"]["password"] = this->wifiPassword;
    }

    // string cfg;
    // serializeJson(config, cfg);
    string ssid = config["wifi"]["ssid"];
    string wifiPassword = config["wifi"]["password"];

    if (this->ssid != ssid || this->wifiPassword != wifiPassword)
    {
        this->ssid = ssid;
        this->wifiPassword = wifiPassword;
        WiFiHandler::BeginSTA(this->ssid.c_str(), this->wifiPassword.c_str());
    }

    ClearDevices();
    if (config["devices"].is<JsonObject>())
    {
        JsonObject jsonDevices = config["devices"].as<JsonObject>();
        for (JsonObject::iterator it = jsonDevices.begin(); it != jsonDevices.end(); ++it)
        {
            if (it->value().is<JsonObject>())
            {
                Device* newDevice = Device::GenerateDevice(it->value().as<JsonObject>());
                if (newDevice != nullptr)
                {
                    devices[it->key().c_str()] = newDevice;
                }
            }
        }
    }
    File file = LittleFS.open(configFilePath, FILE_WRITE);
    if (!file)
    {
        return;
    }
    serializeJson(config, file);
    file.close();
}
//!
//! @brief Execute logic of devices
//!
void HardwareHandler::Execute()
{
    for (std::map<string, Device*>::iterator it = devices.begin(); it != devices.end(); it++)
    {
        it->second->Execute();
    }
}
//!
//! @brief Update setpoints presented by json
//!
void HardwareHandler::UpdateTargetValues(JsonObject values)
{
    JsonObject targetValues = values["devices"].as<JsonObject>();
    for (JsonObject::iterator it = targetValues.begin(); it != targetValues.end(); ++it)
    {
        Device* device = GetDevice(it->key().c_str());
        if (device != nullptr)
        {
            device->SetTarget(it->value());
        }
    }
}
//!
//! @brief Update hardware-config/Setpoints
//!
void HardwareHandler::Update(JsonObject config)
{
    if (!config["config"].isNull())
    {
        UpdateHardwareConfig(config["config"]);
    }
    if (!config["values"].isNull())
    {
        UpdateTargetValues(config["values"]);
    }
}
//!
//! @brief Builds config from content
//!
string HardwareHandler::GetConfig()
{
    stringstream config;
    config << "{\"wifi\": { \"ssid\": \"" << ssid << "\", \"password\": \"" << wifiPassword << "\" },";
    config << "\"devices\": {";
    for (std::map<string, Device*>::iterator it = devices.begin(); it != devices.end(); it++)
    {
        if (it != devices.begin())
        {
            config << ",";
        }
        config << "\"" << it->first << "\": {" << it->second->GetConfig() << "}";
    }
    config << "}}";
    return config.str();
}