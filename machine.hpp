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


enum UARTSTATES{
   u_ENUMURATION,
   u_ENUMURATION_PENDING,
   u_ENUMURATION_STANDBY,
   u_STANDBY,
   u_TRANSFER,

};

enum STATES{
   m_STANDBY,
   m_LOADED,
   m_DISCHARGE,
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

        void startStreaming();
        void startPollingMeters();
        void startEnumeration();
        void resetStages();

        void configureStages(int stageConfig[][4]);
        void checkObstacle();
        void txByte(char);
        void reportStages();
        void armMachine();
        void startFiring();
        void fire();
        uint8_t readReg(uint8_t stage, uint8_t reg);
        void writeReg(uint8_t stage, uint8_t reg, uint8_t data);
    // private:
        uint8_t _readReg(uint8_t stage, uint8_t reg);
        void _writeReg(uint8_t stage, uint8_t reg, uint8_t data);

        void enumerateStages();

        void streamStages();
        void pollMeters();

        void RxIRQ();

        string report();
      // private:
        std::vector<Stage*> stages;

        RawSerial *MasterUART;
        DigitalOut *O1;
        DigitalOut *O2;
        Thread maTh;
        EventQueue *maQueue;
        osThreadId_t mainThreadID;
        enum UARTSTATES _UARTSTATE=u_STANDBY;
        vector<char> rxBuf;
        int _UARTROUTER=0;
};


#endif /* machine_hpp */
