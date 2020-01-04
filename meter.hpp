//
//  meter.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef METER_H
#define METER_H
#define METER_SINGLE_LED_MODE 0
#define METER_DUAL_LED_MODE 1

#include <string>
#include "mbed.h"
#include <map>
#include <regex>
#include <vector>
#include "terminal.hpp"
//#include <NonCopyable.h>


class Meter : NonCopyable<Meter>{
  public:
    Meter(PinName, PinName);
    float getVelocity();
    void resetMeter();

  private:
    void cpuISR();

    void cpdISR();
    void arm(){
      armed=true;
      timer.reset();
    }
    // void assignTrigger(Callback<void()>);
    // Callback<void()> triggerFunc;

    InterruptIn* cp; // checkpoint 1
    InterruptIn* cp2; // checkpint 2
    Timer timer;
    bool resultReady=false;
    int cpu=0; //check point 1 positive edge
    int cpd=0; //check point 1 negative edge
    bool mode=METER_SINGLE_LED_MODE;//

    bool armed=false;// must arm to avoid picking up ramdom stuff
friend class Stage;
};


#endif
