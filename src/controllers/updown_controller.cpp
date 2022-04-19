//
// Created by lbjsy on 2022/4/15.
//

#include "updown_controller.h"

#define UP_THRESHOLD 1000
#define DOWN_THRESHOLD 3000
#define TIME_FIRE_KEEP 1000
#define KEEP_INTERVAL 500

UpDownController::State x2s(int value){
    if(value<UP_THRESHOLD){
        return UpDownController::State::Up;
    }
    else if(value > DOWN_THRESHOLD){
        return UpDownController::State::Down;
    }
    else{
        return UpDownController::State::None;
    };
}
UpDownController::UpDownController(int xPin,int yPin) {
    _xPin = xPin;
    _yPin = yPin;
}

void UpDownController::processState(State newState,unsigned long time){
    switch (newState) {
        case State::Up:
            if(_state == State::None){
                _lastDownUpKeepTime = time;
            }
            else if(_state == State::Up){
                if(_keeping){
                    if(time - _lastDownUpKeepTime > KEEP_INTERVAL){
                        Serial.println("fire up keep");
                        _lastDownUpKeepTime = time;
                        fire(UpDownEvent::KeepUpOne);
                    }
                }else{
                    if(time - _lastDownUpKeepTime > TIME_FIRE_KEEP){
                        Serial.println("start up keep");
                        _lastDownUpKeepTime = time;
                        _keeping = true;
                        fire(UpDownEvent::StartKeepUp);
                        fire(UpDownEvent::KeepUpOne);
                    }
                }
            }
            else if(_state == State::Down){
                processState(State::None,time);
                processState(State::Down,time);
            }
            break;
        case State::Down:
            if(_state == State::None){
                _lastDownUpKeepTime = time;
            }
            else if(_state == State::Down){
                if(_keeping){
                    if(time - _lastDownUpKeepTime > KEEP_INTERVAL){
                        _lastDownUpKeepTime = time;
                        fire(UpDownEvent::KeepDownOne);
                    }
                }else{
                    if(time - _lastDownUpKeepTime > TIME_FIRE_KEEP){
                        _lastDownUpKeepTime = time;
                        _keeping = true;
                        fire(UpDownEvent::StartKeepDown);
                        fire(UpDownEvent::KeepDownOne);
                    }
                }
            }
            else if(_state == State::Up){
                processState(State::None,time);
                processState(State::Up,time);
            }
            break;
        case State::None:
            if(_keeping){
                if(_state == State::Down){
                    fire(UpDownEvent::StopKeepDown);
                }
                else if(_state == State::Up){
                    fire(UpDownEvent::StopKeepUp);
                }
                _keeping = false;
            }else{
                if(_state == State::Down){
                    fire(UpDownEvent::DownOne);
                }
                else if(_state == State::Up){
                    fire(UpDownEvent::UpOne);
                }
            }
            break;
        default:
            break;
    }
    _state = newState;
}

void UpDownController::Poll(unsigned long time) {
    auto x = analogRead(_xPin);
    auto newState = x2s(x);
    _x = x;
    processState(newState,time);
}

int UpDownController::Interval() {
    return 30;
}
void UpDownController::Init() {
    pinMode(_xPin,INPUT);
    pinMode(_yPin,INPUT);
    _state = State::None;
}
