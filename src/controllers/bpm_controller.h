//
// Created by lbjsy on 2022/4/16.
//

#ifndef UNTITLED2_BPM_CONTROLLER_H
#define UNTITLED2_BPM_CONTROLLER_H

#include "../controller.h"
class BpmController: public Controller{
public:
    BpmController(int readPin,int writePin);

    void Poll(unsigned long time) override;

    int Interval() override;
public:
    int Bpm() const{
        return _bpm;
    }
    int Count()const {
        return _count;
    }

    void Init() override;
    void Add();
private:
    int _readPin;
    int _writePin;
    int _bpm;
    int _count;
    uint32_t _lastTime = 0;
    std::list<uint32_t> _times;
};


#endif //UNTITLED2_BPM_CONTROLLER_H
