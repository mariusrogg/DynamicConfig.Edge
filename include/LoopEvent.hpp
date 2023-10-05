//!
//! @file LoopEvent.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Event for periodic tasks
//!
//! @copyright Copyright (c) 2023
//!
#pragma once

#include "EventHandling.hpp"
#include "Arduino.h"

namespace ModelController
{
    class LoopEvent
    {
        public:
            class LoopListener : public Event<>::Listener
            {
                private:
                    //!
                    //! @brief Minimum timeout between two calls in seconds
                    //!
                    double timeout = 0;
                    //!
                    //! @brief Time of the last call in seconds
                    //!
                    double lastCall = 0;
                    //!
                    //! @brief Callback called periodically
                    //!
                    std::function<void()> callback;
                    //!
                    //! @brief Method called by loop to call callback
                    //!
                    void call()
                    {
                        // Check if callback needs to be called (time since last call is higher oder equal timeout)
                        if (lastCall == 0 || (millis() / 1000.0) - lastCall >= timeout)
                        {
                            callback();
                            lastCall = millis() / 1000.0;
                        }
                    }

                public:
                    //!
                    //! @brief Construct a new Loop Listener object
                    //!
                    //! @param callback Callback called periodically
                    //! @param timeout Minimum timeout between two calls in secondsMinimum timeout between two calls in seconds
                    //!
                    LoopListener(std::function<void()> callback,  double timeout = 0)
                        : Listener(&loopEvent, [&](){ this->call(); }),
                        timeout(timeout),
                        callback(callback)
                    { }
            };
        private:
            //!
            //! @brief Event to be raised on loop
            //!
            static Event<> loopEvent;
            //!
            //! @brief Empty ctor (pure static class)
            //!
            LoopEvent();
        public:
            //!
            //! @brief Raise loop event (needs to be called by 'loop()')
            //!
            static void Raise()
            {
                loopEvent.Raise();
            }
    };
} // namespace ModelController
