//
// Created by lbjsy on 2022/4/19.
//

#include "mode_controller.h"
#include <algorithm>
void ModeController::Poll(unsigned long time) {
    if(_started){
        auto elasped = ElapsedTimeSecond();
        auto item = _grades.lower_bound(elasped);
        if(item==_grades.end()){
            Stop();
            fire(ModeEvent::Ended);
        }else if(item->first!=_lastChangeTime){
            _desiredGrade = item->second;
            fire(ModeEvent::DesiredGradeChanged);
        }
    }
}

int ModeController::Interval() {
    return 500;
}

void ModeController::Init() {

}

void ModeController::AddTimeGrade(unsigned long second, int grade) {
    _grades[second] = grade;
}

void ModeController::Start() {
    _startTime = nowSecond();
    _lastChangeTime = 0;
    _started = true;
}
void ModeController::Stop() {
    _started = false;
}

unsigned long ModeController::nowSecond() {
    return millis()/1000;
}

unsigned long ModeController::TotalTimeSecond() {
    if(_grades.empty()){
        return 0;
    }
    return _grades.rbegin()->first;
}

unsigned long ModeController::RemainTimeSecond() {
    if(!_started){
        return 0;
    }
    return TotalTimeSecond()- ElapsedTimeSecond();
}

unsigned long ModeController::ElapsedTimeSecond() {
    if(!_started){
        return 0;
    }
    return nowSecond()-_startTime;
}
