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
#include <map>
#include <regex>
#include <vector>
#include "terminal.hpp"

class Stage{
  public:
    Stage();


    // void armStage();arming is at machine level

    float getVoltage();
    float getCurrent();
    void printStats();
    // commands
    int slaveID=0x00;

    RawSerial * sharedUART;
};

#endif
