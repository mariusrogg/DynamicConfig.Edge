#pragma once
#include "Sequence.hpp"
#include <limits>

//!
//! @brief Single sequence element
//!
class Element : public Sequence
{
private:
    //!
    //! @brief Start value of the element
    //!
    double start;
    //!
    //! @brief Duration of the element
    //!
    long duration;
    //!
    //! @brief End value of the element
    //!
    double end;

protected:
    //!
    //! @brief Get the actual value with relative time (time between now and start)
    //!
    //! @param time Relative time since start of element
    //!
    //! @return double Actual value
    //!
    virtual double GetValueRelativeTime(long time) override;

public:
    //!
    //! @brief Construct a new Element object
    //!
    //! @param start Start value of the element
    //! @param duration Duration of the element
    //! @param end End value of the element
    //! @param repeat Number of repetitions
    //!
    Element(double start, long duration, double end, int repeat = 1);
    //!
    //! @brief Construct a new Element object
    //!
    //! @param start Start value of the element
    //! @param duration Duration of the element
    //! @param repeat Number of repetitions
    //!
    Element(double start, long duration, int repeat = 1);
    //!
    //! @brief Construct a new Element object
    //!
    //! @param start Start value of the element
    //!
    Element(double start);
    //!
    //! @brief Destruction of the Element object
    //!
    virtual ~Element();
    //!
    //! @brief Get the duration of the element execution
    //!
    //! @param withRepeat True if duration is calculated with repetition
    //!
    //! @return long Duration of the element
    //!
    virtual long GetDuration(bool withRepeat = true) const override;
    //!
    //! @brief Get the string of the sequence
    //!
    //! @return string String describing the sequence
    //!
    virtual string GetSequenceString() override;

};