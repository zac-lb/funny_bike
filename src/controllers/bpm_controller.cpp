//
// Created by lbjsy on 2022/4/16.
//

#include "bpm_controller.h"

void static callback(void *p)
{
    auto bpm = (BpmController *)p;
    bpm->Add();
}
void BpmController::Poll(unsigned long time)
{                                                                                          
}
int BpmController::Interval()
{
    return 1000;
}

void BpmController::Add()
{
    bool first = _lastTime == 0;
    auto now = millis();
    auto diff = now - _lastTime;
    if (diff > 200)
    {
        _count++;
        if (!first)
        {
            _bpm = 60000 / diff;
            _times.push_back(diff);
            if (_times.size() > 3)
            {
                _times.pop_front();
            }
        }
    }

    _lastTime = now;
}

void BpmController::Init()
{
    pinMode(_readPin, INPUT_PULLDOWN);
    attachInterruptArg(_readPin, callback, this, FALLING);
    pinMode(_writePin, OUTPUT);
    digitalWrite(_writePin, HIGH);
}

BpmController::BpmController(int readPin,int writePin) : _readPin(readPin),_writePin(writePin) {}
