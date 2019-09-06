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
    Stage(serialTerminal* theTerm ,int in_id, Meter* pre_meter, PinName voltage, PinName current,PinName out_trigger, PinName in_trigger=NC);

    // meta data
    int getID();
    int getState(); // 0 idle, 1 triggered, 2 discharging
    float getVoltage();
    float getCurrent();
    void printStats();
    // commands
    void armStage(); // 1 success, 0 fail

    void setDelay(int inputDelay){triggerDelay=inputDelay;}
    void setUndelay(int inputDelay){untriggerDelay=inputDelay;} // undelay == the delay from neg edge to shutoff
    void setWidth(int inputWidth){pulseWidth=inputWidth;}
    void widthModeEnable(bool enable){widthMode=enable;}
    void posedgeEnable(bool enable){posEdge=enable;}
    void forceTrigger();

    int getDelay(){return triggerDelay;}
    int getUndelay(){return triggerUndelay;}
    int getWidth(){return pulseWidth;}
  private:
    void triggerISR();
    void discharge();
    void donothing();
    void driveHigh();
    void driveLow();
    Meter* meter;

    bool armed=false;
    bool widthMode=false;// default to trigger + delay mode
    bool posEdge=true;// by default, use positive edge to trigger unless other noted, this is for stage1 w/ force trigger
    int state=0;

    int id;

    DigitalOut*   triggerO;
    InterruptIn*  triggerI;
    AnalogIn*     voltageADC;
    AnalogIn*     currentADC;
    serialTerminal* theTerm;

    int triggerDelay=100;
    int triggerUndelay=0;
    int pulseWidth=500;

    Timeout outPosEdge;
    Timeout outNegEdge;
    friend class meter;
};

#endif
