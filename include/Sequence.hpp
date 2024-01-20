//!
//! @file Sequence.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Class for calculating values depending on time and a given string
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <vector>
#include <string>

namespace ModelController
{
    //!
    //! @brief Class for calculation of actual values by given string
    //!
    class Sequence
    {
    public:
        //!
        //! @brief Subsequences of the sequence
        //!
        std::vector<Sequence*> subsequences;
        //!
        //! @brief Number of repetitions of the sequence
        //!
        int repeat;

    protected:
        //!
        //! @brief Get the actual value with relative time (time between now and start)
        //!
        //! @param time Relative time since start of element
        //!
        //! @return double Actual value
        //!
        virtual double GetValueRelativeTime(long time);
        //!
        //! @brief Initialize sequence with repetition
        //!
        //! @param repeat Number of repetitions of the sequence
        //!
        Sequence(int repeat = 1);

    public:
        //!
        //! @brief Construct a new Sequence object
        //!
        //! @param sequenceString Configuration of the sequence
        //!
        Sequence(std::string sequenceString);
        //!
        //! @brief Destruction of the Sequence object
        //!
        virtual ~Sequence();
        //!
        //! @brief Get the duration of the sequence execution
        //!
        //! @param withRepeat True if duration is calculated with repetition
        //!
        //! @return long Duration of the sequence
        //!
        virtual long GetDuration(bool withRepeat = true) const;
        //!
        //! @brief Get the number of repetitions of the sequence
        //!
        //! @return int Number of repetitions
        //!
        int GetRepeat() const;
        //!
        //! @brief Get the configuration of the sequence as string
        //!
        //! @return string Configuration of the sequence
        //!
        virtual std::string GetSequenceString();
        //!
        //! @brief Get the value by actual absolute time
        //!
        //! @param time Acutal time
        //!
        //! @return double Actual value
        //!
        double GetValue(unsigned long time);
    };
} // namespace ModelController
