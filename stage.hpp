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


#define CMD_PING 0x0f
#define CK_CMD_PING(data) (CMD_PING==(data&0x0f) ? 1:0)
#define CMD_ACK 0x01
#define CK_CMD_ACK(data) (CMD_ACK==(data&0x0f) ? 1:0)
#define CMD_RST 0x02
#define CK_CMD_RST(data) (CMD_RST==(data&0x0f) ? 1:0)
#define CMD_ENUM 0x03
#define CK_CMD_ENUM(data) (CMD_ENUM==(data&0x0f) ? 1:0)
#define CMD_CONT 0x04// continue enum
#define CK_CMD_CONT(data) (CMD_CONT==(data&0x0f) ? 1:0)
#define CMD_ARM 0x05
#define CK_CMD_ARM(data) (CMD_ARM==(data&0x0f) ? 1:0)
#define CMD_DISARM 0x06
#define CK_CMD_DISARM(data) (CMD_DISARM==(data&0x0f) ? 1:0)
#define CMD_STREAM 0x07
#define CK_CMD_STREAM(data) (CMD_STREAM==(data&0x0f) ? 1:0)
#define CMD_METER 0x08
#define CK_CMD_METER(data) (CMD_METER==(data&0x0f) ? 1:0)
#define CMD_FIRE 0x09
#define CK_CMD_FIRE(data) (CMD_FIRE==(data&0x0f) ? 1:0)
#define CMD_REGR 0x0a
#define CK_CMD_REGR(data) (CMD_REGR==(data&0x0f) ? 1:0)
#define CMD_REGW 0x0b
#define CK_CMD_REGW(data) (CMD_REGW==(data&0x0f) ? 1:0)

#define TxPack(TxADDR,TxCMD) ((char)(TxADDR<<4)+TxCMD)

#define SID(RxByte)  (RxByte>>4)
#define CMD(RxByte)  (RxByte&0x0f)
#define ForMe(RxByte) (((SID(RxByte) == stage_id) | (SID(RxByte) == 0)) ? 1:0)


#define ACK_FLAG 3
#define DONE_FLAG 4

#define METER_WIDTH 71120 // um

enum RxStates{
  s_STREAMING,
  s_METERING,
  s_PING,
  s_IDLE
};

class Stage{
  public:
    Stage(char,Callback<void(char)>);

    void reset();
    void processRx(char bytein);
    void arm();
    void disarm();
    void stream();
    float getSpeed();
    void fire();
    void setThread(Thread*);

    Callback<void(char)> txByte;
    // void armStage();arming is at machine level


    bool ACCEPTING=false;


    void printStats();
    // commands
    int slaveID;
    enum RxStates _RxState = s_IDLE;
    Thread* stageTh;

    uint16_t xfCnt=0;
    uint8_t ADCBuff[128]={0};
    uint8_t streamLn=0;
    uint8_t meterL=0x00;
    uint8_t meterH=0x00;


};

#endif
