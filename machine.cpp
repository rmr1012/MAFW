//
//  machine.cpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#include <stdio.h>
#include "machine.hpp"
// string Inarticulate::buffer;
Machine::Machine(PinName MTX,PinName MRX,PinName pO1,PinName pO2){
  MasterUART = new RawSerial(MTX,MRX,500000);
  MasterUART->attach(callback(this,&Machine::RxIRQ));
  O1= new DigitalOut(pO1);
  O2= new DigitalOut(pO2);

  maQueue= new EventQueue(32 * EVENTS_EVENT_SIZE);
  maTh.start(callback(maQueue, &EventQueue::dispatch_forever));
  mainThreadID= ThisThread::get_id();
};

void Machine::armMachine(){
  for (auto stage : stages){
    stage->arm();
  }
}

void Machine::txByte(char byteout){
  MasterUART->putc(byteout);
  // printf("Tx: 0x%2X\n",byteout);
}

void Machine::streamStages(){
  for (auto stage : stages){
    stage->stream();
  }
}

void Machine::pollMeters(){
  for (auto stage : stages){
    printf("%.4f\n m/s",stage->getSpeed());
  }
}

void Machine::fire(){
  stages[0]->fire();
}
uint8_t Machine::readReg(uint8_t stage, uint8_t reg){
  // printf("reading...\n");
  uint8_t data = stages[stage-1]->readReg(reg);
  return data;
}
void  Machine::writeReg(uint8_t stage, uint8_t reg, uint8_t data){
  // printf("writing...\n");
  stages[stage-1]->writeReg(reg,data);
}


void Machine::enumerateStages(){
  printf("Starting enumeration\n");
  _UARTSTATE=u_ENUMURATION;

  while(1){
    ThisThread::flags_clear(1);
    ThisThread::flags_clear(2);
    O1->write(1); // signal first slave to wake
    O2->write(1);
    ThisThread::flags_wait_any_for(1, 5);
    O2->write(0);
    if(_UARTSTATE!=u_ENUMURATION_PENDING){
      printf("Waiting for slave response timed out\n");
      O1->write(0);
      break;//abaort
    }
    char asnSlaveID=stages.size()+1;
    txByte(TxPack(asnSlaveID,CMD_ENUM));

    ThisThread::flags_wait_any_for(2, 5);
    if(_UARTSTATE!=u_ENUMURATION){
      printf("Waiting for slave response timed out 2\n");
      O1->write(0);
      break;//abaort
    }
    printf("Stage assigned ID 0x%X\n",asnSlaveID);
    Stage* newStage = new Stage(asnSlaveID,callback(this,&Machine::txByte));
    // newStage->setThread(&maTh);
    stages.push_back(newStage);

    O1->write(0);


  }
  printf("Enum complete\n");
  _UARTSTATE=u_STANDBY;
}
void Machine::resetStages(){
  MasterUART->putc(TxPack(0x0,CMD_RST));
}

void Machine::RxIRQ(){

  while(MasterUART->readable()){
    char theChar=(char) MasterUART->getc();
    // rxBuf.push_back(theChar);
    if (_UARTSTATE==u_ENUMURATION){
      if(CK_CMD_PING(theChar)){
          // maTh.flags_set(1);
          osSignalSet(mainThreadID, 1);
        _UARTSTATE=u_ENUMURATION_PENDING;
      }
      else{
        printf("Got unexpected response during enum %X\n",theChar);
        _UARTSTATE=u_STANDBY;
      }
    }
    else if (_UARTSTATE==u_ENUMURATION_PENDING){
      if(CK_CMD_ACK(theChar)){
         // maTh.flags_set(2);
         osSignalSet(mainThreadID, 2);
         printf("Ack detected\n");
        _UARTSTATE=u_ENUMURATION;
      }
      else{
        printf("Got unexpected response during enum 2 %X\n",theChar);
        _UARTSTATE=u_STANDBY;
      }
    }
    else{
      if(_UARTSTATE==u_TRANSFER){
        if(stages[_UARTROUTER]->ACCEPTING)
          stages[_UARTROUTER]->processRx(theChar);
        else
          _UARTSTATE=u_STANDBY;
      }

      if( (SID(theChar)!=0) & (_UARTSTATE==u_STANDBY)){
        _UARTROUTER=SID(theChar)-1;
        // printf("r2 %i",_UARTROUTER);
        stages[_UARTROUTER]->processRx(theChar);
        _UARTSTATE=u_TRANSFER;
      }
    }
  }
}


void Machine::reportStages(){
  printf("Stage\tv(m/s)\tdN(us)\tdP(us)\tdOff(us)\tSTO(ms)\n");
  for(auto stage : stages) {
    stage->report();
  }
}
void Machine::configureStages(int stageConfig[][4]){
  int index=0;
  for(auto stage : stages) {
    stage->setOnDelayN(stageConfig[index][0]);
    stage->setOnDelayP(stageConfig[index][1]);
    stage->setOffDelay(stageConfig[index][2]);
    stage->setSafetyTO(stageConfig[index][3]);
    index++;
  }
}
void Machine::checkObstacle(){
  for(auto stage : stages) {
    uint8_t data = stage->getObstacles();
    printf("OBSTACLE %X\n",data);
  }
}
//
// string Machine::report(){
//   string report="==== STATE MACHINE ====\n";
//   report += current->report();
//   return report;
// }
