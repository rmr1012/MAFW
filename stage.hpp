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
#include "trigger.hpp"
#include <map>
#include <regex>
#include <vector>
#include "terminal.hpp"

class Stage{
  public:
    Stage(PinName output,PinName voltage, PinName current);


    // void armStage();arming is at machine level

    float getVoltage();
    float getCurrent();
    void printStats();
    // commands
    void assignTrigger(Trigger* in_trigger);
    void assignMeter(Meter* in_meter);

    void forceTrigger();
    Meter* meter;
    Trigger* trigger;

  private:
    void donothing();
    void driveHigh();
    void driveLow();


    bool armed=false;
    bool widthMode=false;// default to trigger + delay mode
    bool posEdge=true;// by default, use positive edge to trigger unless other noted, this is for stage1 w/ force trigger


    DigitalOut*   FETgate;

    AnalogIn*     voltageADC;
    AnalogIn*     currentADC;


    friend class Meter;
    friend class Trigger;
};

#endif
