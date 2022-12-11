#include "HWDevice.hpp"
#include "OnboardPWM.hpp"
#include <sstream>

using namespace std;

namespace ModelController
{
    std::map<string, HWDevice*> HWDevice::devices;
    HWDevice* HWDevice::rootDevice = nullptr;
    HWDevice::HWDevice(string name, JsonObject config, HWDevice* parent)
        : parent(parent),
        name(name),
        path((GetParent() != nullptr ? GetParent()->GetPath() : "") + "/" + GetName())
    {
        devices[GetPath()] = this;
        if (parent)
        {
            parent->children.push_back(this);
        }
    }
    HWDevice::~HWDevice()
    {
        if (parent)
        {
            remove(parent->children.begin(), parent->children.end(), this);
        }
    }
    HWDevice* HWDevice::GetParent() const
    {
        return parent;
    }
    string HWDevice::GetPath() const
    {
        return path;
    }
    string HWDevice::GetName() const
    {
        return name;
    }
    HWDevice* HWDevice::GenerateDevice(string name, JsonObject deviceConfig, HWDevice* parent)
    {
        HWDevice* device = nullptr;
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
    string HWDevice::GetConfig()
    {
        stringstream config;
        for (std::vector<HWDevice*>::iterator it = children.begin(); it != children.end(); it++)
        {
            if (it != children.begin())
            {
                config << ",";
            }
            config << "\"" << (*it)->GetName() << "\": {" << (*it)->GetConfig() << "}";
        }
        return config.str();
    }
    template<class T>
    T* HWDevice::GetDevice(std::string devicePath)
    {
        HWDevice* device = nullptr;
        if (devices.find(devicePath) != devices.end())
        {
            device = devices[devicePath];
        }
        return dynamic_cast<T*>(device);
    }
    void HWDevice::UpdateConfig(JsonObject config)
    {
        if (rootDevice != nullptr)
        {
            delete rootDevice;
            rootDevice = nullptr;
        }
        // ToDo: Generate config from json
    }
} // namespace ModelController
