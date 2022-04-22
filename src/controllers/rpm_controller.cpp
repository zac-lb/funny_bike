//
// Created by lbjsy on 2022/4/16.
//

#include "rpm_controller.h"
#define AVG_COUNT 5

static int weight[AVG_COUNT] = {20,40,60,80,100};

void static callback(void *p)
{
    auto bpm = (RpmController *)p;
    bpm->Add();
}
void RpmController::Poll(unsigned long time)
{
    auto timeNoUpdate = time - _lastTime;
    if(timeNoUpdate<=3000){
        if(_state!=State::Started){
            changeState(State::Started);
        }
    }
    else {
        _rmp = 0;
        if(_state!= State::Paused){
            changeState(State::Paused);
        }
    }
}
int RpmController::Interval()
{
    return 1000;
}

void RpmController::Add()
{
    bool first = _lastTime == 0;
    auto now = millis();
    auto diff = now - _lastTime;
    if (diff > 200)
    {
        _count++;
        if (!first)
        {
            _times.push_back(diff);
            if (_times.size() > AVG_COUNT)
            {
                _times.pop_front();
            }
            auto rbegin = _times.rbegin();
            uint32_t allWeight = 0;
            uint32_t allValue = 0;
            for(int i=AVG_COUNT-1;i>=0&&rbegin!=_times.rend();i--){
                allWeight+=weight[i];
                allValue+=(*rbegin)*weight[i];
            }
            if(allWeight>0){
                _rmp = 6000 / (allValue / allWeight);
            }
        }
    }
    _lastTime = now;
}

void RpmController::Init()
{
    pinMode(_readPin, INPUT_PULLDOWN);
    attachInterruptArg(_readPin, callback, this, FALLING);
    pinMode(_writePin, OUTPUT);
    digitalWrite(_writePin, HIGH);
}

RpmController::RpmController(int readPin, int writePin) : _readPin(readPin), _writePin(writePin) {}

void RpmController::changeState(RpmController::State newState) {
    if(_state != newState){
        _state = newState;
        fire(RpmEvent::StateChanged);
    }
}
