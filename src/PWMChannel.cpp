//!
//! @file PWMChannel.cpp
//! @author Marius Roggenbuck (marius.rog@gmx.de)
//! @brief Class for simpler acessing microcontroller's pins
//!
//! @copyright Copyright (c) 2020
//!
#include <Arduino.h>
#include "PWMChannel.hpp"
#include <math.h>
#include <string>
#include <sstream>

//!
//! @brief PWM-Channels already used by the program
//!
vector<int> PWMChannel::usedChannels;
//!
//! @brief Iterates from 0 to max. number of channels, returns first free channel number
//!
int PWMChannel::GetFreeChannel()
{
  int channel = -1;
  for (int i = 0; i < numberChannels - 1 && channel < 0; i++)
  {
    if (std::find(usedChannels.begin(), usedChannels.end(), i) == usedChannels.end())
    {
      channel = i;
    }
  }
  return channel;
}
//!
//! @brief Resetup the channel, detach pins and attach pins to new channel
//!
void PWMChannel::SetChannel(int frequency, int resolution)
{
  this->resolution = resolution;
  if (channel >= 0)
  {
    ledcSetup(channel, frequency, resolution);
    for (std::map<string, int>::iterator it = pins.begin(); it != pins.end(); it++)
    {
        ledcDetachPin(it->second);
        ledcAttachPin(it->second, channel);
    }
  }
}
//!
//! @brief Construct a new PWMChannel object
//!
PWMChannel::PWMChannel(JsonObject pins)
{
  this->channel = GetFreeChannel();
  usedChannels.push_back(this->channel);
  for (JsonObject::iterator it = pins.begin(); it != pins.end(); ++it)
  {
    if (it->value().is<int>())
    {
      this->pins[it->key().c_str()] = it->value().as<int>();
    }
  }
  SetChannel();
}
//!
//! @brief Destroy the PWMChannel object
//!
//! Detach pins from channel
//!
PWMChannel::~PWMChannel()
{
  SetValue(0);
  for (std::map<string, int>::iterator it = pins.begin(); it != pins.end(); it++)
  {
      ledcDetachPin(it->second);
  }
}
//!
//! @brief Set the channels duty value
//!
void PWMChannel::SetValue(double value)
{
  int duty = value / 100 * (pow(2, resolution) - 1);
  if (channel >= 0)
  {
    ledcWrite(channel, duty);
    actualValue = value;
  }
}
//!
//! @brief Returns actual value
//!
double PWMChannel::GetValue() const
{
  return actualValue;
}
//!
//! @brief Returns json with attached pins
//!
string PWMChannel::GetConfig()
{
  stringstream config;
  for (std::map<string, int>::iterator it = pins.begin(); it != pins.end(); it++)
  {
    if (!config.str().empty())
    {
      config << ",";
    }
    config << "\"" << it->first << "\": " << it->second;
  }
  return config.str();
}