#pragma once

#include "Device.hpp"
#include "PWMChannel.hpp"
#include "Mode.hpp"
#include <string>
#include <map>

using namespace std;

class DevicePWM : public Device
{
private:
    //!
    //! @brief PWM-Channel
    //!
    PWMChannel channel;
    //!
    //! @brief Possible modes
    //!
    std::map<string, Mode*> modes;
    //!
    //! @brief Name of the default mode
    //!
    string defaultMode;
    //!
    //! @brief Active mode
    //!
    Mode* activeMode = nullptr;
    //!
    //! @brief Next activated mode
    //!
    Mode* nextActiveMode = nullptr;
    //!
    //! @brief Sequence on turning mode off
    //!
    Sequence off;
    //!
    //! @brief Sequence on turning mode on
    //!
    Sequence on;
    //!
    //! @brief Time where mode was started
    //!
    long timeStartMode = -1;
    //!
    //! @brief Get the time where on sequence is started
    //!
    //! @return long Time where on sequence is started
    //!
    long GetTimeStartOn() const;
    //!
    //! @brief Time where mode was set to end
    //!
    long timeEndMode = -1;
    //!
    //! @brief Get the time where off sequence is ended
    //!
    //! @return long Time where off sequence is ended
    //!
    long GetTimeEndOff() const;
    //!
    //! @brief True if mode is active
    //!
    bool active = true;
    //!
    //! @brief True if mode should be active in next cycle
    //!
    bool targetActive = false;
    //!
    //! @brief Get the mode by its name
    //!
    //! @param name Name of the mode
    //! @return Mode* Mode with specified name
    //!
    Mode* GetMode(string name);
    //!
    //! @brief Manual set target value, -1 if none is set
    //!
    double manualTarget = -1;

public:
    //!
    //! @brief Name of the type for config
    //!
    static constexpr const char* type = "pwm";
    //!
    //! @brief Construct a new DevicePWM
    //!
    //! @param deviceConfig Json with configuration of the device
    //!
    DevicePWM(JsonObject deviceConfig);
    //!
    //! @brief Destruction of the Object
    //!
    ~DevicePWM();
    //!
    //! @brief Set the target value for PWM-Device
    //!
    //! Possibilities:
    //! Bool:
    //! True - Activate last active mode or default if none was active
    //! False - Deactivate active mode
    //! String:
    //! Activate mode by string (turns off last mode and on given mode)
    //! Double:
    //! Set target value of PWM-Channel
    //!
    //! @param targetValue Target value
    //!
    virtual void SetTarget(JsonVariant targetValue) override;
    //!
    //! @brief Execute channel logic to calculate output
    //!
    virtual void Execute() override;
    //!
    //! @brief Get the config of the object as json
    //!
    //! @return string Config
    //!
    virtual string GetConfig() override;
};