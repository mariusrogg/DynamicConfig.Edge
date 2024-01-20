//!
//! @file EventHandling.hpp
//! @author Marius Roggenbuck (roggenbuckmarius@gmail.com)
//! @brief Event and listener for event driven programming
//!
//! @copyright Copyright (c) 2023
//!
#pragma once
#include <list>
#include <functional>
#include <iterator>
#include "Logger.hpp"

namespace ModelController
{
    template<typename... T>
    class Event
    {
        public:
            class Listener
            {
                private:
                    //!
                    //! @brief Event, on which listeners callback is called
                    //!
                    Event<T...>* event;
                    //!
                    //! @brief Callback called on event
                    //!
                    std::function<void(T...)> callback;
                public:
                    //!
                    //! @brief Construct a new Listener object
                    //!
                    //! Listener is added to event's listeners
                    //!
                    //! @param event Event, on which listeners callback is called
                    //! @param callback Callback called on event
                    //!
                    Listener(Event<T...>* event, std::function<void(T...)> callback)
                        : callback(callback),
                        event(event)
                    {
                        (*event) += this;
                    }
                    //!
                    //! @brief Destruction of the Listener object
                    //!
                    //! Listener is removed from event's listeners
                    //!
                    ~Listener()
                    {
                        (*event) -= this;
                    }
                    //!
                    //! @brief Fuction called from Event, calls callback
                    //!
                    //! @param args Parameters passed from event to callback
                    //!
                    void call(T... args)
                    {
                        callback(args...);
                    }
            };
        private:
            //!
            //! @brief Listeners to be called on raised event
            //!
            std::list<Listener*> listeners;
            //!
            //! @brief Callbacks to be called on raised event
            //!
            std::list<void(*)(T...)> callbacks;
        public:
            //!
            //! @brief Construct a new Event object
            //!
            Event(){}
            //!
            //! @brief Add listener to listeners
            //!
            //! @param listener Listener to be added
            //!
            void AddListener(Listener* listener)
            {
                listeners.push_back(listener);
                Logger::trace("Listener added to event");
            }
            //!
            //! @brief Add callback to callbacks
            //!
            //! @param callback Callback to be added
            //!
            void AddCallback(void(*callback)(T...))
            {
                callbacks.push_back(callback);
                Logger::trace("Callback added to event");
            }
            //!
            //! @brief Remove listener to listeners
            //!
            //! @param listener Listener to be removed
            //!
            void RemoveListener(Listener* listener)
            {
                listeners.remove(listener);
                Logger::trace("Listener removed from event");
            }
            //!
            //! @brief Remove callback to callbacks
            //!
            //! @param callback Callback to be removed
            //!
            void RemoveCallback(void(*callback)(T...))
            {
                callbacks.remove(callback);
                Logger::trace("Callback removed from event");
            }
            //!
            //! @brief Raise event and call each listeners callback
            //!
            //! @param args Parameters to be passed to callback
            //!
            void Raise(T ... args)
            {
                for (typename std::list<Listener*>::reverse_iterator i = listeners.rbegin(); i != listeners.rend(); i++)
                {
                    (*i)->call(args...);
                }
                for (typename std::list<void(*)(T...)>::reverse_iterator i = callbacks.rbegin(); i != callbacks.rend(); i++)
                {
                    (*i)(args...);
                }
            }
            //!
            //! @brief Operator += to add listener
            //!
            //! @param listener Listener to be added
            //! @return Event<T...>& Reference of this object
            //!
            Event<T...>& operator+=(Listener* listener)
            {
                AddListener(listener);
                return *this;
            }
            //!
            //! @brief Operator += to add callback
            //!
            //! @param callback Callback to be added
            //! @return Event<T...>& Reference of this object
            //!
            Event<T...>& operator+=(void(*callback)(T...))
            {
                AddCallback(callback);
                return *this;
            }
            //!
            //! @brief Operator -= to remove listener
            //!
            //! @param listener Listener to be removed
            //! @return Event<T...>& Reference of this object
            //!
            Event<T...>& operator-=(Listener* listener)
            {
                RemoveListener(listener);
                return *this;
            }
            //!
            //! @brief Operator -= to remove callback
            //!
            //! @param callback Callback to be removed
            //! @return Event<T...>& Reference of this object
            //!
            Event<T...>& operator-=(void(*callback)(T...))
            {
                RemoveCallback(callback);
                return *this;
            }
            //!
            //! @brief Operator () to raise event
            //!
            //! @param args Parameters to be passed to listeners/callbacks
            //! @return Event<T...>& Reference of this object
            //!
            Event<T...>& operator()(T... args)
            {
                Raise(args...);
                return *this;
            }
    };
} // namespace ModelController
