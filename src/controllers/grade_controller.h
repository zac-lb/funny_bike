//
// Created by lbjsy on 2022/4/15.
//

#ifndef UNTITLED2_GRADE_CONTROLLER_H
#define UNTITLED2_GRADE_CONTROLLER_H

#include "../controller.h"
#include "../core.h"

class GradeController : public Controller {

    enum class RunState{
        None = 0,
        Up,
        Down,
    };
public:
    GradeController(int anglePin,int relayPin1,int relayPin2);

    void Poll(unsigned long time) override;

    int Interval() override;
    void Init() override;

    int UpOneGrade();
    int DownOneGrade();

public:
    int CurrentGrade() const { return _currentGrade; }

    int TargetGrade() const { return _targetGrade; }

    int MaxGrade() const { return _maxGrade; }

    void SetTargetGrade(int grade);


private:
    void changeRunState(RunState state);
private:
    int _currentGrade = 0;
    int _targetGrade = 0;
    int _maxGrade = 20;
    int _rPin = -1;
    int _relayPin1 = -1;
    int _relayPin2 = -1;

    RunState _state = RunState::None;
};


#endif //UNTITLED2_GRADE_CONTROLLER_H
