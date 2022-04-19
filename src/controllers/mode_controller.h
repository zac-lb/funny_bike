//
// Created by lbjsy on 2022/4/19.
//

#ifndef FUNNY_BIKE_MODE_CONTROLLER_H
#define FUNNY_BIKE_MODE_CONTROLLER_H

#include "../controller.h"
#include <map>
enum class ModeEvent{
    DesiredGradeChanged,
    Ended,
};
class ModeController : public EventController<ModeEvent>{
public:
    void Poll(unsigned long time) override;

    int Interval() override;

    void Init() override;

public:
    void Start();
    void Stop();
    unsigned long TotalTimeSecond();
    unsigned long RemainTimeSecond();
    unsigned long ElapsedTimeSecond();
public:
    void AddTimeGrade(unsigned long second,int grade);
    int DesiredGrade() const{
        return _desiredGrade;
    }

private:
    unsigned long nowSecond();
private:
     std::map<unsigned long,int> _grades;//time:grade
     int _desiredGrade = 0;
     unsigned long _startTime = 0;
     bool _started = false;
     unsigned long _lastChangeTime = 0;
};

#endif //FUNNY_BIKE_MODE_CONTROLLER_H