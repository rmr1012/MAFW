//
//  trigger.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef TRIGGER_H
#define TRIGGER_H
#define TRIGGER_ARMED 0
#define TRIGGER_WORKING 1
#define TRIGGER_SPENT 2


#include <string>
#include "mbed.h"

class Trigger{
  public:
    Trigger(PinName , PinName );


    void posEdgeISR();
    void negEdgeISR();
    void assignPos(Callback<void()>);
    void assignNeg(Callback<void()>);
    void assignPosP(Callback<void()>);
    void assignNegP(Callback<void()>);
    void assignSpent(Callback<void()>);

    Callback<void()> posEdgeSignal;
    Callback<void()> negEdgeSignal;
    void posEdgeSignalWrap(){posEdgeSignal();}
    void negEdgeSignalWrap(){negEdgeSignal();negEdgeSignalP();state=TRIGGER_SPENT;triggerSpentCallback();}

    Callback<void()> triggerSpentCallback;
    Callback<void()> posEdgeSignalP;
    Callback<void()> negEdgeSignalP;
    void posEdgeSignalWrapP(){posEdgeSignalP();}

    void setMode(bool in_mode){mode=in_mode;}
    void setPosDelay(int delay){posDelay=delay;}
    void setNegDelay(int delay){negDelay=delay;}
    void setPosDelayP(int delay){posDelayP=delay;}
    void setNegDelayP(int delay){negDelayP=delay;}
    void setPulseWidth(int width){pulseWidth=width;}

    void armTrigger();

    PinName triggerPin;
    PinName triggerPin2;

    InterruptIn*  triggerI;
    InterruptIn*  triggerI2;
    int posDelay=1;
    int negDelay=0;
    int posDelayP=1;// delay calculated from when trig 1 delay sends pos signal
    int negDelayP=0;
    int pulseWidth=500;

    Timeout outPosEdgeTO;
    Timeout outNegEdgeTO;
    Timeout outPosEdgeTOP;
    Timeout outNegEdgeTOP;

    Timeout safetyTO;
    int safeTime=20; // 20ms max pulse width or times out

    bool mode=0; //0 trigger+delay negedge, 1 delay only neg edge

    int state=TRIGGER_SPENT;
};
#endif
