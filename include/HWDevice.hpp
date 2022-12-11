//!
//! @file HWDevice.hpp
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
    class HWDevice
    {
    private:
        //!
        //! @brief List with all devices in programm and their paths
        //!
        static std::map<std::string, HWDevice*> devices;
        //!
        //! @brief Root device of the hardware configuration
        //!
        static HWDevice* rootDevice;
        //!
        //! @brief Parent of the device object
        //!
        HWDevice* parent;
        //!
        //! @brief Children of the device object
        //!
        std::vector<HWDevice*> children;
        //!
        //! @brief Path of the device object
        //!
        std::string path;
        //!
        //! @brief Name of the device object
        //!
        std::string name;

    public:
        //!
        //! @brief Construct a new HWDevice object
        //!
        //! @param name Name of the device
        //! @param config Config of the device
        //! @param parent Parent of the HWDevice (normally pass this)
        //!
        HWDevice(std::string name, JsonObject config, HWDevice* parent = nullptr);
        //!
        //! @brief Destroy the HWDevice object
        //!
        virtual ~HWDevice();
        //!
        //! @brief Get the Parent of the object
        //!
        //! @return HWDevice* Parent of the object
        //!
        HWDevice* GetParent() const;
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
        //! @return HWDevice* Created device
        //!
        static HWDevice* GenerateDevice(std::string name, JsonObject deviceConfig, HWDevice* parent = nullptr);
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
        //! @return T* HWDevice with path
        //!
        //! @brief Get a device by path
        //!
        //! @tparam T Expected type of the device
        //! @param devicePathPath of the device
        //! @return T* Device with given path
        //!
        template<class T>
        static T* GetDevice(std::string devicePath);
        //!
        //! @brief Update hardware configuration
        //!
        //! @param config Json object with new hardware config
        //!
        static void UpdateConfig(JsonObject config);
    };
} // namespace ModelController
