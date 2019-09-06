//
//  meter.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef METER_H
#define METER_H

#include <string>
#include "mbed.h"
#include <map>
#include <regex>
#include <vector>
#include "terminal.hpp"
//#include <NonCopyable.h>


class Meter : NonCopyable<Meter>{
  public:
    Meter(serialTerminal* theTerm ,int ,PinName );
    int getID();
    float getVelocity();
    void resetMeter();

  private:
    void cpuISR();

    void cpdISR();

    // void assignTrigger(Callback<void()>);
    // Callback<void()> triggerFunc;

    int id;
    InterruptIn* cp1;
    Timer timer;
    serialTerminal* theTerm;

    int cp1u; //check point 1 positive edge
    int cp1d; //check point 1 negative edge

friend class Stage;
};


#endif
