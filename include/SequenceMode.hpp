//!
//! @file SequenceMode.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Sequence mode containing Sequence, synchronization information and flags indicating, if on/off sequence needs to get called
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include "Sequence.hpp"
#include "ArduinoJson.h"

namespace ModelController
{
    //!
    //! @brief SequenceMode for controlling value
    //!
    class SequenceMode
    {
    private:
        //!
        //! @brief Sequence of the mode
        //!
        Sequence sequence;
        //!
        //! @brief On-Sequence is executed on start
        //!
        bool on = false;
        //!
        //! @brief Off-Sequence is executed after end
        //!
        bool off = false;
        //!
        //! @brief Sequence is syncronized
        //!
        long syncTime = 0;

    public:
        //!
        //! @brief Construct a new SequenceMode object
        //!
        //! @param mode Configuration of the mode
        //!
        SequenceMode(JsonObject mode);
        //!
        //! @brief Destruction of the SequenceMode object
        //!
        ~SequenceMode();
        //!
        //! @brief Get the actual value of the mode depending on time
        //!
        //! @param time Time for which value is needed
        //! @return double Actual value
        //!
        double GetValue(unsigned long time);
        //!
        //! @brief Get sync time of the mode
        //!
        //! @return long Sync time of the mode
        //!
        long GetSyncTime() const;
        //!
        //! @brief Get flag if on sequence is used by mode
        //!
        //! @return true On sequence is used
        //! @return false On sequence is not used
        //!
        bool GetOn() const;
        //!
        //! @brief Get flag if off sequence is used by mode
        //!
        //! @return true Off sequence is used
        //! @return false Off sequence is not used
        //!
        bool GetOff() const;
        //!
        //! @brief Get the config of the mode as json
        //!
        //! @return string Config of the mode
        //!
        std::string GetConfig();
    };

} // namespace ModelController