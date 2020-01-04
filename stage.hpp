//
//  meter.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef STAGE_H
#define STAGE_H

#define ADC2AMP 66
#define ADC2VOLT 22

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
    Stage(PinName output,PinName outputP,PinName voltage, PinName current);


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
    void driveHighP();
    void driveLowP();
    void record();

    float currentVals[100]={};
    float voltageVals[100]={};
    int recordCounter=0;
    Timeout recordTO;
    Thread ADCThread;
    osThreadId ADCThreadID;
    bool armed=false;
    bool widthMode=false;// default to trigger + delay mode
    bool posEdge=true;// by default, use positive edge to trigger unless other noted, this is for stage1 w/ force trigger
    float jules=0;
    float kinetic=0;
    int recordInterval=100;

    DigitalOut*   FETgate;
    DigitalOut*   FETgateP;

    AnalogIn*     voltageADC;
    AnalogIn*     currentADC;


    friend class Meter;
    friend class Trigger;
};

#endif
