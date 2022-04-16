//
// Created by lbjsy on 2022/4/16.
//

#ifndef UNTITLED2_BPM_CONTROLLER_H
#define UNTITLED2_BPM_CONTROLLER_H

#include "../controller.h"
class BpmController: public Controller{
public:
    void Poll(unsigned long time) override;

    int Interval() override;

private:
    int _pin;
};


#endif //UNTITLED2_BPM_CONTROLLER_H
