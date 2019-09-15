//
//  trigger.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef TRIGGER_H
#define TRIGGER_H

#include <string>
#include "mbed.h"

class Trigger{
  public:
    Trigger(PinName triggerPin, bool first=false);// first stage is special


    void posEdgeISR();
    void negEdgeISR();
    void assignPos(Callback<void()>);
    void assignNeg(Callback<void()>);
    Callback<void()> posEdgeSignal;
    Callback<void()> negEdgeSignal;
    void posEdgeSignalWrap(){posEdgeSignal();}
    void negEdgeSignalWrap(){negEdgeSignal();}

    void setMode(bool);
    void setPosDelay(int);
    void setNegDelay(int);
    void setPulseWidth(int);

    PinName triggerPin;

    InterruptIn*  triggerI;
    int posDelay=1;
    int negDelay=0;
    int pulseWidth=500;

    Timeout outPosEdgeTO;
    Timeout outNegEdgeTO;

    bool mode=0; //0 trigger+delay negedge, 1 delay only neg edge
};


#endif
