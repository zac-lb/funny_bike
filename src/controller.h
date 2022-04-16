//
// Created by lbjsy on 2022/4/15.
//

#ifndef UNTITLED2_CONTROLLER_H
#define UNTITLED2_CONTROLLER_H

#include <map>
#include <list>
#include <Arduino.h>
#include <U8g2lib.h>
#include <unordered_map>
#include <map>
#include <list>
#include <functional>

class Core;

class Controller {
public:
    virtual void Poll(unsigned long time) = 0;
    virtual int Interval() = 0;
    virtual void Init() = 0;
    Core* GetCore(){
        return _core;
    }
    void SetCore(Core* core){
        _core = core;
    }
protected:
    Core* _core;
};

template<class EventType>
class EventController : public Controller {
public:
    virtual void On(EventType event, std::function<void()> &&callback) {
        _eventHandlers[event].push_back(callback);
    }

protected:
    virtual void fire(EventType event) {
        auto handlers = _eventHandlers.find(event);
        if (handlers == _eventHandlers.end()) {
            return;
        }
        std::for_each(handlers->second.begin(), handlers->second.end(), [](std::function<void()> &func) {
            func();
        });
    }

protected:
    std::map<EventType, std::list<std::function<void()>>> _eventHandlers;

};

#endif //UNTITLED2_CONTROLLER_H
