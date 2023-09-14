//!
//! @file Device.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the device
//! @version 0.0.0
//! @date 2023-09-14
//!
//! @copyright Copyright (c) 2023
//!
#include "Device.hpp"
#include <sstream>
#include "LittleFS.h"
#include "OnboardPWM.hpp"
#include "WiFiHandler.hpp"

using namespace std;

namespace ModelController
{
    //!
    //! @brief List with all devices of the controller
    //!
    std::map<string, Device*> Device::devices;
    //!
    //! @brief Path to the configuration path
    //!
    std::string Device::configFilePath = Device::defaultConfigFile;
    //!
    //! @brief Root device
    //!
    Device* Device::rootDevice = nullptr;
    //!
    //! @brief Construct a new Device object
    //!
    Device::Device(string name, JsonObject config, Device* parent)
        : parent(parent),
        name(name),
        path((GetParent() != nullptr ? GetParent()->GetPath() : "") + "/" + name)
    {
        Serial.println(GetPath().c_str());
        devices[GetPath()] = this;
        if (parent)
        {
            parent->children.push_back(this);
        }

        for (JsonPair child : config)
        {
            // ToDo: Only dive in GenerateDevie if child.value() is object
            Serial.println(child.key().c_str());
            GenerateDevice(child.key().c_str(), child.value(), this);
        }
    }
    //!
    //! @brief Destruction the Device object
    //!
    Device::~Device()
    {
        if (parent)
        {
            remove(parent->children.begin(), parent->children.end(), this);
        }
    }
    //!
    //! @brief Returns parent of the actual object
    //!
    Device* Device::GetParent() const
    {
        return parent;
    }
    //!
    //! @brief Returns path of the actual object
    //!
    string Device::GetPath() const
    {
        return path;
    }
    //!
    //! @brief Returns name of the actual object
    //!
    string Device::GetName() const
    {
        return name;
    }
    //!
    //! @brief Generates device from json
    //!
    Device* Device::GenerateDevice(string name, JsonObject deviceConfig, Device* parent)
    {
        Device* device = nullptr;
        if (deviceConfig["type"].is<string>())
        {
            string type = deviceConfig["type"].as<string>();
            if (type == OnboardPWM::type)
            {
                device = new OnboardPWM(name, deviceConfig, parent);
            }
        }
        return device;
    }
    //!
    //! @brief Get config of the device and it's children
    //!
    string Device::GetConfig()
    {
        stringstream config;
        for (std::vector<Device*>::iterator it = children.begin(); it != children.end(); it++)
        {
            if (it != children.begin())
            {
                config << ",";
            }
            config << "\"" << (*it)->GetName() << "\": {" << (*it)->GetConfig() << "}";
        }
        return config.str();
    }
    //!
    //! @brief Update config of the controller
    //!
    void Device::UpdateConfig(JsonObject config)
    {
        if (rootDevice != nullptr)
        {
            delete rootDevice;
            rootDevice = nullptr;
        }

        string hwName = "HW";
        JsonObject cnf(config);


        if (cnf["wifi"]["ssid"].isNull())
        {
            cnf["wifi"]["ssid"] = WiFiHandler::GetSSID();
        }
        if (cnf["wifi"]["password"].isNull())
        {
            cnf["wifi"]["password"] = WiFiHandler::GetPassword();
        }

        WiFiHandler::SetSSIDPassword(cnf["wifi"]["ssid"], cnf["wifi"]["password"]);

        rootDevice = new Device(hwName, cnf);

        std::string hwConfig = "{ \"" + rootDevice->GetName() + "\": {" + rootDevice->GetConfig() + "}}";

        // ToDo: Override file on change
        // File file = LittleFS.open(Device::configFilePath.c_str(), FILE_WRITE);
        // if (!file)
        // {
        //     return;
        // }
        // file.print(hwConfig.c_str());
        // file.close();
    }
    //!
    //! @brief Initially build config
    //!
    void Device::InitConfig(std::string configFilePath)
    {
        Device::configFilePath = configFilePath;
        File file = LittleFS.open(Device::configFilePath.c_str(), FILE_READ);
        DynamicJsonDocument jsonDoc(32768);

        deserializeJson(jsonDoc, file);

        file.close();
        UpdateConfig(jsonDoc.as<JsonObject>());
    }
} // namespace ModelController
