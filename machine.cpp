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

};

void Machine::armMachine(){
  for (auto stage : stages){
    stage->arm();
  }
}

void Machine::txByte(char byteout){
  MasterUART->putc(byteout);
  printf("Tx: 0x%2X\n",byteout);
}
void Machine::startEnumeration(){
  stages.clear();
  maQueue->call(callback(this,&Machine::enumerateStages));
}
void Machine::startStreaming(){
  maQueue->call(callback(this,&Machine::streamStages));
}
void Machine::streamStages(){
  for (auto stage : stages){
    stage->stream();
  }
}
void Machine::startPollingMeters(){
  maQueue->call(callback(this,&Machine::pollMeters));//dummy
}
void Machine::pollMeters(){
  for (auto stage : stages){
    printf("%.4f\n m/s",stage->getSpeed());
  }
}
void Machine::startFiring(){
  maQueue->call(callback(this,&Machine::fire));
}
void Machine::fire(){
  stages[0]->fire();
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
    newStage->setThread(&maTh);
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
          maTh.flags_set(1);
        _UARTSTATE=u_ENUMURATION_PENDING;
      }
      else{
        printf("Got unexpected response during enum %X\n",theChar);
        _UARTSTATE=u_STANDBY;
      }
    }
    else if (_UARTSTATE==u_ENUMURATION_PENDING){
      if(CK_CMD_ACK(theChar)){
         maTh.flags_set(2);
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
void xferComplete(){
  printf("Transfer complete, token returned\n");

}
void clearBuff(){

}

// void Machine::reportStages(){
//   printf("   \tStage\tDelay\tWidth\tV\tI\tSpeed\n");
//   for(std::vector<int>::size_type i = 0; i != stages.size(); i++) {
//     printf("%i\t",i);
//     stages[i]->printStats();
//   }
// }
//
// string Machine::report(){
//   string report="==== STATE MACHINE ====\n";
//   report += current->report();
//   return report;
// }
