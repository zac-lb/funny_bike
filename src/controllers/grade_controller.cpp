//
// Created by lbjsy on 2022/4/15.
//

#include "grade_controller.h"

#define TICK_PER_GRADE 200
#define MAX_VALUE 3200
int a2g(int a)
{
    return (a - TICK_PER_GRADE + 1) / TICK_PER_GRADE + 1;
}
int threshold(int grade)
{
    return (TICK_PER_GRADE * grade) - (TICK_PER_GRADE / 2);
}

GradeController::GradeController(int anglePin, int relayPin1, int relayPin2)
{
    _rPin = anglePin;
    _relayPin1 = relayPin1;
    _relayPin2 = relayPin2;
    _maxGrade = MAX_VALUE / TICK_PER_GRADE;
}

void GradeController::Poll(unsigned long time)
{
    if(_stable){
        return;
    }
    auto thresholdValue = threshold(_targetGrade);
    auto nowValue = analogRead(_rPin);
    if(_state == RunState::None){
        if(std::abs(nowValue-thresholdValue)>TICK_PER_GRADE/2){
            if(nowValue<thresholdValue){
                changeRunState(RunState::Up);
            }else{
                changeRunState(RunState::Down);
            }
        }else{
            _stable = true;
        }
    }else if(_state == RunState::Up){
        if(nowValue>=thresholdValue){
            changeRunState(RunState::None);
        }
    }else if(_state == RunState::Down){
        if(nowValue<=thresholdValue){
            changeRunState(RunState::None);
        }
    }
}

int GradeController::Interval()
{
    return 30;
}

void GradeController::Init()
{
    pinMode(_rPin, INPUT_PULLDOWN);
    pinMode(_relayPin1, OUTPUT);
    pinMode(_relayPin2, OUTPUT);
    _targetGrade = 1;
    _stable = false;
}

void GradeController::changeRunState(RunState state)
{
    if (state == _state)
    {
        return;
    }
    else if (state == RunState::Up)
    {   
        Serial.println("start up");
        digitalWrite(_relayPin1, LOW);
        digitalWrite(_relayPin2, HIGH);
    }
    else if (state == RunState::Down)
    {
        Serial.println("start down");
        digitalWrite(_relayPin1, HIGH);
        digitalWrite(_relayPin2, LOW);
    }
    else
    {
        Serial.println("none");
        digitalWrite(_relayPin1, LOW);
        digitalWrite(_relayPin2, LOW);
    }
    _state = state;
}

int GradeController::UpOneGrade()
{
    if (_targetGrade < _maxGrade)
    {
        SetTargetGrade(_targetGrade + 1);
        return 0;
    }
    return -1;
}

void GradeController::SetTargetGrade(int grade)
{
    if (grade > 0 && grade <= _maxGrade&&_targetGrade!=grade)
    {
        _targetGrade = grade;
        _stable = false;
        Poll(millis());
    }
}
int GradeController::CurrentValue(){
    return analogRead(_rPin);
}
int GradeController::DownOneGrade()
{
    if (_targetGrade > 1)
    {
        SetTargetGrade(_targetGrade - 1);
        return 0;
    }
    return -1;
}
