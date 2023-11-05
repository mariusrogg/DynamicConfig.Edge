//!
//! @file Sequence.cpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Implementation of the sequence
//!
//! @copyright Copyright (c) 2023
//!
#include "Sequence.hpp"
#include "SequenceElement.hpp"
#include <Arduino.h>
#include <string>
#include <sstream>

namespace ModelController
{
    //!
    //! @brief Get actual value depending on relative time
    //!
    double Sequence::GetValueRelativeTime(long time)
    {
        double value = 0;
        long timeCounter = 0;

        if (GetDuration(false) > 0)
        {
            time = time % GetDuration(false);
        }
        //!
        //! @brief Get value from active subsequence
        //!
        for (int i = 0; i < subsequences.size() && timeCounter < time; i++)
        {

            if (timeCounter + subsequences[i]->GetDuration() > time || subsequences[i]->GetDuration() < 0)
            {
                value = subsequences[i]->GetValueRelativeTime(time - timeCounter);
                timeCounter = time;
            }
            else
            {
                timeCounter += subsequences[i]->GetDuration();
            }
        }
        return value;
    }
    //!
    //! @brief Construct a new Sequence object
    //!
    Sequence::Sequence(int repeat)
        : repeat(repeat)
    {

    }
    //!
    //! @brief Construct a new Sequence object
    //!
    Sequence::Sequence(std::string sequenceString)
    {
        std::string repeat = sequenceString.substr(0, sequenceString.find("{"));
        if (repeat == "inf")
        {
            this->repeat = -1;
        }
        else if(repeat == "")
        {
            this->repeat = 1;
        }
        else
        {
            this->repeat = atoi(repeat.c_str());
        }
        std::string subsequenceString = sequenceString.substr(sequenceString.find("{") + 1, sequenceString.find_last_of("}") - sequenceString.find("{") - 1);
        int deep = 0;
        int index = 0;
        int start = index;

        while (index < subsequenceString.length())
        {
            if (subsequenceString[index] == '{')
            {
                deep++;
            }
            else if (subsequenceString[index] == '}' || index == subsequenceString.length() - 1)
            {
                if (--deep <= 0)
                {
                    std::string subsubsequenceString = subsequenceString.substr(start, index - start + 1);
                    if (subsubsequenceString.find("{") != std::string::npos)
                    {
                        subsequences.push_back(new Sequence(subsubsequenceString));
                    }
                    else
                    {
                        std::string startStr = "";
                        std::string timeStr = "";
                        std::string endStr = "";
                        if (subsubsequenceString.find(";") != std::string::npos)
                        {
                            startStr = subsubsequenceString.substr(0, subsubsequenceString.find(";"));
                            subsubsequenceString = subsubsequenceString.substr(subsubsequenceString.find(";") + 1, subsubsequenceString.length() - subsubsequenceString.find(";"));
                            if (subsubsequenceString.find(";") != std::string::npos)
                            {
                                timeStr = subsubsequenceString.substr(0, subsubsequenceString.find(";"));
                                endStr = subsubsequenceString.substr(subsubsequenceString.find(";") + 1, subsubsequenceString.length() - subsubsequenceString.find(";"));
                            }
                            else
                            {
                                timeStr = subsubsequenceString;
                            }
                        }
                        else
                        {
                            startStr = subsubsequenceString;
                        }
                        if (endStr != "")
                        {
                            subsequences.push_back(new SequenceElement(std::atof(startStr.c_str()), std::atol(timeStr.c_str()), std::atof(endStr.c_str())));
                        }
                        else if (timeStr != "")
                        {
                            subsequences.push_back(new SequenceElement(std::atof(startStr.c_str()), std::atol(timeStr.c_str())));
                        }
                        else if (startStr != "")
                        {
                            subsequences.push_back(new SequenceElement(std::atof(startStr.c_str())));
                        }

                    }
                    start = index + 1;
                }
            }
            index++;
        }
    }
    //!
    //! @brief Destruction of the sequence object
    //!
    Sequence::~Sequence()
    {
        for (Sequence* subsequence : subsequences)
        {
            delete subsequence;
        }
        subsequences.clear();
    }
    //!
    //! @brief Get actual value from actual time
    //!
    double Sequence::GetValue(unsigned long time)
    {
        double value = 0;
        if (GetDuration() <= 0)
        {
            value = GetValueRelativeTime(time);
        }
        else
        {
            value = GetValueRelativeTime(time % GetDuration());
        }
        return value;
    }
    //!
    //! @brief Returns number of repetitions
    //!
    int Sequence::GetRepeat() const
    {
        return repeat;
    }
    //!
    //! @brief Calculates duration of the sequence
    //!
    long Sequence::GetDuration(bool withRepeat) const
    {
        long duration = 0;

        if (withRepeat && repeat < 0)
        {
            duration = -1;
        }
        else
        {
            for (int i = 0; i < subsequences.size() && duration >= 0; i++)
            {
                if (subsequences[i]->GetDuration() >= 0)
                {
                    duration += subsequences[i]->GetDuration();
                }
                else
                {
                    duration = -1;
                }
            }
            if (withRepeat)
            {
                duration *= repeat;
            }
        }
        return duration;
    }
    //!
    //! @brief Calculate the sequence's config string
    //!
    std::string Sequence::GetSequenceString()
    {
        std::stringstream sequenceString;
        if (GetRepeat() < 0)
        {
            sequenceString << "inf";
        }
        else if (GetRepeat() == 1)
        {
            sequenceString << "";
        }
        else
        {
            sequenceString << GetRepeat();
        }
        sequenceString << "{";
        for (int i = 0; i < subsequences.size(); i++)
        {
            sequenceString << subsequences[i]->GetSequenceString();
        }
        sequenceString << "}";
        return sequenceString.str();
    }

} // namespace ModelController
