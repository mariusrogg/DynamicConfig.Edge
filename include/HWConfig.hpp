#pragma once
#include <map>
#include <string>
#include <ArduinoJson.h>
#include "Device.hpp"

namespace ModelController
{
    class HWConfig
    {
    private:
        static HWDevice* rootDevice;
        ~HWConfig();
        HWConfig();
        static void ClearDevices();

    public:
        static HWDevice* GetDevice(std::string devicePath);
        static void UpdateHWConfig(JsonObject config);
    };
} // namespace ModelController
