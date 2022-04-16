//
// Created by lbjsy on 2022/4/15.
//

#include "core.h"
#include "controller.h"
void Core::AddController(Controller *controller)
{
    auto interval = controller->Interval();
    _controllers[interval].push_back(controller);
    if(_controllers[interval].size()==1){
        auto now = millis();
        _timers[now + interval] = interval;
    }
    controller->SetCore(this);
}

void Core::RemoveController(Controller *controller)
{
    auto item = _controllers.find(controller->Interval());
    if (item != _controllers.end())
    {
        for (auto b = item->second.begin(); b != item->second.end(); b++)
        {
            if (*b == controller)
            {
                item->second.erase(b);
                controller->SetCore(nullptr);
                break;
            }
        }
    }
}

void Core::Poll(unsigned long time)
{
    auto begin = _timers.begin();
    std::list<unsigned long> removed;
    while (begin != _timers.end())
    {
        if (begin->first > time)
        {
            break;
        }
        auto interval = begin->second;
        //poll controller;
        auto controller = _controllers.find(interval);
        if (controller != _controllers.end())
        {
            std::list<Controller *>::iterator it;
            for (it = controller->second.begin(); it != controller->second.end(); it++)
            {
                (*it)->Poll(time);
            }
        }
        removed.push_back(begin->second);
        begin = _timers.erase(begin);
    }
    for (auto b = removed.begin(); b != removed.end(); b++)
    {
        auto interval = *b;
        _timers[time + interval] = interval;
    }
    {//loop delay callbacks
        auto until = _delayCallbacks.upper_bound(time);
        for(auto item = _delayCallbacks.begin();item!=until;item=_delayCallbacks.erase(item)){
            for(auto& f:item->second){
                f();
            }
        }
    }
    {//loop period callbacks;
        auto until = _periodicallyCallbacks.upper_bound(time);
        std::list<std::pair<uint32_t,std::function<void()>>> reAdd;
        for(auto item = _periodicallyCallbacks.begin();item!=until;item=_periodicallyCallbacks.erase(item)){
            for(auto& f:item->second){
                f.second();
                reAdd.push_back(f);
            }
        }
        for(auto& item:reAdd){
            _periodicallyCallbacks[item.first+time].push_back(item);
        }
    }
}

void Core::DelayCall(uint32_t delay, std::function<void()> &&callback) {
    auto expired = millis()+delay;
    _delayCallbacks[expired].push_back(callback);
}

void Core::PeriodCall(uint32_t interval, std::function<void()> &&callback) {
    auto expired = millis()+interval;
    _periodicallyCallbacks[expired].push_back(std::make_pair(interval,callback));
}
