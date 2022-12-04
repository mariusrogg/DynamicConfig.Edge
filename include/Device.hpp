#pragma once

#include <ArduinoJson.h>
#include <string>

using namespace std;

class Device
{
private:

protected:
    Device(/* args */);

public:
    virtual ~Device();
    static Device* GenerateDevice(JsonObject deviceConfig);
    virtual void SetTarget(JsonVariant targetValue) = 0;
    virtual string GetConfig() = 0;
    virtual void Execute() = 0;
};