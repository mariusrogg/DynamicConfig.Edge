#include "Device.hpp"

#include "DevicePWM.hpp"
Device::Device(/* args */)
{
}

Device::~Device()
{
}
Device* Device::GenerateDevice(JsonObject deviceConfig)
{
    Device* device = nullptr;
    if (deviceConfig["type"].is<string>())
    {
        string type = deviceConfig["type"].as<string>();
        if (type == DevicePWM::type)
        {
            device = new DevicePWM(deviceConfig);
        }
    }
    return device;
}