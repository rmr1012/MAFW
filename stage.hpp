//
//  meter.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef STAGE_H
#define STAGE_H

#include <string>
#include "mbed.h"
#include "meter.hpp"
#include <map>
#include <regex>
#include <vector>
#include "terminal.hpp"

class Stage{
  public:
    Stage(serialTerminal* theTerm ,int in_id, Meter* pre_meter, float in_delay, PinName voltage, PinName current,PinName out_trigger, PinName in_trigger=NC);

    // meta data
    int getID();
    int getState(); // 0 idle, 1 triggered, 2 discharging
    float getVoltage();
    float getCurrent();
    void printStats();
    // commands
    void armStage(); // 1 success, 0 fail

    void setDelay(int inputDelay){triggerDelay=inputDelay;}
    void setWidth(int inputWidth){pulseWidth=inputWidth;}

    int getDelay(){return triggerDelay;}
    int getWidth(){return pulseWidth;}
  private:
    void triggerISR();
    void discharge();
    void donothing();
    void driveHigh();
    void driveLow();
    Meter* meter;

    bool armed=false;
    int state=0;

    int id;
    float delay;

    DigitalOut*   triggerO;
    InterruptIn*  triggerI;
    AnalogIn*     voltageADC;
    AnalogIn*     currentADC;
    serialTerminal* theTerm;

    int triggerDelay=100;
    int pulseWidth=500;

    Timeout outPosEdge;
    Timeout outNegEdge;
    friend class meter;
};

#endif
