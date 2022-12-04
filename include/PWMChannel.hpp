#pragma once
//!
//! @file PWMChannel.hpp
//! @author Marius Roggenbuck (marius.rog@gmx.de)
//! @brief Class for simpler acessing microcontroller's pins
//!
//! @copyright Copyright (c) 2020
//!
#include "ArduinoJson.h"
#include <map>
#include <vector>

using namespace std;

//!
//! @brief Class for simpler accessing microcontroller's pins
//!
class PWMChannel
{
  private:
    //!
    //! @brief PWM-Channels already used by the program
    //!
    static vector<int> usedChannels;
    //!
    //! @brief Amount of channels on the controller
    //!
    static constexpr int numberChannels = 16;
    //!
    //! @brief Number of the channel
    //!
    int channel;
    //!
    //! @brief Resolution of the channel
    //!
    int resolution;
    //!
    //! @brief Actual value set to PWM
    //!
    double actualValue = 0;
    //!
    //! @brief Pins attached to the channel
    //!
    std::map<string, int> pins;
    //!
    //! @brief Get the a free channel
    //!
    //! @return int Free channel
    //!
    static int GetFreeChannel();
    //!
    //! @brief Reset the channels parameters
    //!
    //! @param frequency   Cycle Frequency of the channel
    //! @param resolution  Resolution of the channel
    //!
    void SetChannel(int frequency = DefaultFrequency, int resolution = DefaultResolution);

  public:
    //!
    //! @brief Default cycle frequency of the channel
    //!
    static const int DefaultFrequency = 5000;
    //!
    //! @brief Default resolution of the channel
    //!
    static const int DefaultResolution = 8;
    //!
    //! @brief Construct a new PWMChannel object
    //!
    //! @param pins Json with pins to be added to the channel
    //!
    PWMChannel(JsonObject pins);
    //!
    //! @brief Destroy the PWMChannel object
    //!
    ~PWMChannel();
    //!
    //! @brief Set the channels duty value
    //!
    //! @param value   Duty value
    //!
    void SetValue(double value);
    //!
    //! @brief Get the actual value set to channel
    //!
    //! @return double Actual value set to channel
    //!
    double GetValue() const;
    //!
    //! @brief Get attached pins
    //!
    //! @return string Json with attached pins
    //!
    string GetConfig();
};