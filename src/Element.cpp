#include "Element.hpp"
#include <Arduino.h>
#include <sstream>

//!
//! @brief Calculate actual value depending on time
//!
double Element::GetValueRelativeTime(long time)
{
    //!
    //! @brief If duration is bigger than time, modulo is calculated
    //!
    time = time % duration;
    //!
    //! @brief If time is smaller zero (infinity) start is returned
    //!
    if (time <= 0)
    {
        return start;
    }
    //!
    //! @brief If time is bigger than duration end is returned
    //!
    else if (time >= duration)
    {
        return end;
    }
    //!
    //! @brief Else value is calculated from difference between start and end and the duration and actual time
    //!
    else
    {
        return start + (end - start) * ((double)time / (double)duration);
    }
}
//!
//! @brief Construct a new Element object
//!
Element::Element(double start, long duration, double end, int repeat)
    : Sequence(repeat),
      start(start),
      duration(duration),
      end(end)
{
}
//!
//! @brief Construct a new Element object
//! End is not present => end = start
//!
Element::Element(double start, long duration, int repeat)
    : Element(start, duration, start, repeat)
{
}
//!
//! @brief Construct a new Element object
//! End is not present => end = start
//! Duration is not present => duration = -1 (Infinity)
//! Repeat is not present => repeat = -1 (Infinity)
//!
Element::Element(double start)
    : Element(start, -1, start, -1)
{
}
//!
//! @brief Destruction of the element
//!
Element::~Element()
{
}
//!
//! @brief Calculates duration of the element
//!
long Element::GetDuration(bool withRepeat) const
{
    //!
    //! @brief -1 (Infinity), if repeat smaller zero
    //!
    if (GetRepeat() < 0)
    {
        return - 1;
    }
    else
    {
        //!
        //! @brief If with repeat, duration multiplied with repetition
        //!
        if (withRepeat)
        {
            return duration * GetRepeat();
        }
        else
        {
            return duration;
        }
    }
}
//!
//! @brief Calculate the element's sequence string
//!
string Element::GetSequenceString()
{
    stringstream sequenceString;
    sequenceString << start;
    if (duration >= 0)
    {
        sequenceString << ";" << duration;
        if (end != start)
        {
            sequenceString << ";";
            sequenceString << end;
        }
    }
    return sequenceString.str();
}