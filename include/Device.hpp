//!
//! @file Device.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Base class of Hardware devices
//! @version 0.0.0
//! @date 2022-12-10
//!
//! @copyright Copyright (c) 2022
//!
//!
#pragma once

#include <ArduinoJson.h>
#include <string>
#include <vector>
#include <map>

namespace ModelController
{
    class Device
    {
    private:
        //!
        //! @brief List with all devices in programm and their paths
        //!
        static std::map<std::string, Device*> devices;
        //!
        //! @brief Root device of the hardware configuration
        //!
        static Device* rootDevice;
        //!
        //! @brief Parent of the device object
        //!
        Device* parent;
        //!
        //! @brief Children of the device object
        //!
        std::vector<Device*> children;
        //!
        //! @brief Path of the device object
        //!
        std::string path;
        //!
        //! @brief Name of the device object
        //!
        std::string name;
        //!
        //! @brief Default config file path
        //!
        static std::string configFilePath;

    public:
        //!
        //! @brief Default config file path
        //!
        static constexpr const char* defaultConfigFile = "/HWConfig.json";
        //!
        //! @brief Construct a new Device object
        //!
        //! @param name Name of the device
        //! @param config Config of the device
        //! @param parent Parent of the Device (normally pass this)
        //!
        Device(std::string name, JsonObject config, Device* parent = nullptr);
        //!
        //! @brief Destroy the Device object
        //!
        virtual ~Device();
        //!
        //! @brief Get the Parent of the object
        //!
        //! @return Device* Parent of the object
        //!
        Device* GetParent() const;
        //!
        //! @brief Get the path of the object
        //!
        //! @return std::string Path of the object
        //!
        std::string GetPath() const;
        //!
        //! @brief Get the name of the object
        //!
        //! @return std::string Name of the object
        //!
        std::string GetName() const;
        //!
        //! @brief Generate device from json
        //!
        //! @param deviceConfig Config of the device
        //! @return Device* Created device
        //!
        static Device* GenerateDevice(std::string name, JsonObject deviceConfig, Device* parent = nullptr);
        //!
        //! @brief Get the config of the device
        //!
        //! @return string Config created
        //!
        virtual std::string GetConfig();
        //!
        //! @brief Get a device by path
        //!
        //! @param devicePath Path of the device
        //! @return T* Device with path
        //!
        //! @brief Get a device by path
        //!
        //! @tparam T Expected type of the device
        //! @param devicePathPath of the device
        //! @return T* Device with given path
        //!
        template<class T>
        static T* GetDevice(std::string devicePath)
        {
            Device* device = nullptr;
            if (devices.find(devicePath) != devices.end())
            {
                device = devices[devicePath];
            }
            return dynamic_cast<T*>(device);
        }
        //!
        //! @brief Update hardware configuration
        //!
        //! @param config Json object with new hardware config
        //!
        static void UpdateConfig(JsonObject config);
        //!
        //! @brief Initialize hardware configuration
        //!
        static void InitConfig(std::string configFilePath = defaultConfigFile);
    };
} // namespace ModelController