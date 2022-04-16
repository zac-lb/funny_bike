//
// Created by lbjsy on 2022/4/15.
//

#ifndef UNTITLED2_UPDOWN_CONTROLLER_H
#define UNTITLED2_UPDOWN_CONTROLLER_H
#include "../controller.h"
#include "../core.h"
#include <functional>
enum class UpDownEvent{
    UpOne,
    DownOne,
    StartKeepDown,
    KeepDownOne,
    StopKeepDown,
    StartKeepUp,
    KeepUpOne,
    StopKeepUp,
};
class UpDownController : public EventController<UpDownEvent> {
public:
    enum class State{
        Up,
        Down,
        None,
    };
public:
    UpDownController(int xPin,int yPin);
    void Poll(unsigned long time) override;
    int Interval() override;
    void Init() override;
private:
    int _xValue;
    State _state;
    int _xPin;
    int _yPin;
    unsigned long _lastDownUpKeepTime = 0;
    bool _keeping = false;

    void processState(State newState,unsigned long now);
};


#endif //UNTITLED2_UPDOWN_CONTROLLER_H
