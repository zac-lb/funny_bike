//
// Created by lbjsy on 2022/4/15.
//

#ifndef UNTITLED2_CORE_H
#define UNTITLED2_CORE_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <unordered_map>
#include <map>
#include <list>
#include <functional>
class Controller;
class Core {
public:
    void AddController(Controller* controller);
    void RemoveController(Controller* controller);
    void Poll(unsigned long time);
    void DelayCall(uint32_t delay,std::function<void()>&& callback);
    void PeriodCall(uint32_t interval,std::function<void()>&& callback);
private:
    std::unordered_map<unsigned long,std::list<Controller*>> _controllers;
    std::map<unsigned long,std::list<std::function<void()>>> _delayCallbacks;
    std::map<unsigned long,std::list<std::pair<uint32_t,std::function<void()>>>> _periodicallyCallbacks;
    std::map<clock_t,unsigned long> _timers;
};
#endif //UNTITLED2_CORE_H
