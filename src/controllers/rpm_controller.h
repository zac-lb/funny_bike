//
// Created by lbjsy on 2022/4/16.
//

#ifndef UNTITLED2_BPM_CONTROLLER_H
#define UNTITLED2_BPM_CONTROLLER_H

#include "../controller.h"
enum class RpmEvent{
    StateChanged,
};
class RpmController: public EventController<RpmEvent>{
public:
    enum class State{
        Paused,
        Started,
    };
public:
    RpmController(int readPin, int writePin);

    void Poll(unsigned long time) override;

    int Interval() override;

private:
    void changeState(State newState);
public:
    int Rpm() const{
        return _rmp;
    }
    int Count()const {
        return _count;
    }
    void Init() override;
    void Add();
private:
    int _readPin;
    int _writePin;
    int _rmp;
    int _count;
    uint32_t _lastTime = 0;
    std::list<uint32_t> _times;
    State _state = State::Paused;
};


#endif //UNTITLED2_BPM_CONTROLLER_H
