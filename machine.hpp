//
//  machine.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef MACHINE_H
#define MACHINE_H


#include "mbed.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <utility>
#include "stage.hpp"
#include "utilities.hpp"


enum STATES{
   ENUMURATION,
   ENUMURATION_PENDING,
   ENUMURATION_STANDBY,
   STANDBY,
   LOADED,
   DISCHARGE
};
class Machine{
    public:
        Machine(PinName,PinName,PinName,PinName);
        // InterruptIn* armSwitch;
        // InterruptIn* loadMeter;
        void idle();
        void standby();//aka armed
        void loaded();
        void discharge();

        void txByte(char);
        void reportStages();
        void armMachine();
        void startEnumeration();
        void enumerateStages();
        void RxIRQ();
        void resetStages();
        string report();
      // private:
        std::vector<Stage*> stages;

        RawSerial *MasterUART;
        DigitalOut *O1;
        DigitalOut *O2;
        Thread maTh;
        EventQueue *maQueue;
        enum STATES _STATE=STANDBY;
        vector<char> rxBuf;
};


#endif /* machine_hpp */
