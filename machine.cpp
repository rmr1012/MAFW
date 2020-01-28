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

}

void Machine::txByte(char byteout){
  MasterUART->putc(byteout);
  printf("Tx: 0x%2X\n",byteout);
}
void Machine::startEnumeration(){
  stages.clear();
  maQueue->call(callback(this,&Machine::enumerateStages));
}
void Machine::enumerateStages(){
  printf("Starting enumeration\n");
  _STATE=ENUMURATION;

  while(1){
    O1->write(1); // signal first slave to wake
    O2->write(1);
    ThisThread::flags_wait_any_for(5, 1);
    O2->write(0);
    if(_STATE!=ENUMURATION_PENDING){
      printf("Waiting for slave response timed out\n");
      O1->write(0);
      break;//abaort
    }
    char asnSlaveID=stages.size()+1;
    txByte(TxPack(asnSlaveID,CMD_ENUM));
    ThisThread::flags_wait_any_for(5, 1);
    if(!_STATE==ENUMURATION){
      printf("Waiting for slave response timed out 2\n");
      O1->write(0);
      break;//abaort
    }
    printf("Stage assigned ID 0x%X\n",asnSlaveID);
    Stage* newStage = new Stage(asnSlaveID,callback(this,&Machine::txByte));
    
    stages.push_back(newStage);

    O1->write(0);


  }
  printf("Enum complete\n");
  _STATE=STANDBY;
}
void Machine::resetStages(){
  MasterUART->putc(TxPack(0x0,CMD_RST));
}

void Machine::RxIRQ(){

  while(MasterUART->readable()){
    char theChar=(char) MasterUART->getc();
    printf("Rx: 0x%2X\n",theChar);
    // rxBuf.push_back(theChar);
    if (_STATE==ENUMURATION){
      if(CK_CMD_PING(theChar)){
          maTh.flags_set(1);
        _STATE=ENUMURATION_PENDING;
      }
      else{
        printf("Got unexpected response during enum %X\n",theChar);
        _STATE=STANDBY;
      }
    }
    else if (_STATE==ENUMURATION_PENDING){
      if(CK_CMD_ACK(theChar)){
         maTh.flags_set(2);
         printf("Ack detected\n");
        _STATE=ENUMURATION;
      }
      else{
        printf("Got unexpected response during enum 2 %X\n",theChar);
        _STATE=STANDBY;
      }
    }

  }
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
