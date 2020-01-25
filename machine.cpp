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
  MasterUART->putc(0x55);
};

void Machine::armMachine(){

}
void Machine::startEnumeration(){
  maQueue->call(callback(this,&Machine::enumerateStages));
}
void Machine::enumerateStages(){
  printf("Starting enumeration\n");
  _STATE=ENUMURATION;
  O1->write(1); // signal first slave to wake

  O2->write(1);
  ThisThread::flags_wait_any_for(1, 1);
  O2->write(0);
  if(_STATE!=ENUMURATION_PENDING){
    printf("Waiting for slave response timed out\n");
    O1->write(0);
    return;//abaort
  }
  char asnSlaveID=stages.size()+0xb0+1;
  MasterUART->putc(0x60);
  MasterUART->putc(asnSlaveID);
  MasterUART->putc((char)asnSlaveID+0x60);
  ThisThread::flags_wait_any_for(1, 1);
  if(!_STATE==ENUMURATION){
    printf("Waiting for slave response timed out 2\n");
    O1->write(0);
    return;//abaort
  }
  printf("Stage assigned ID 0x%X\n",asnSlaveID);
  O1->write(0);


  printf("Enum complete\n");

}

void Machine::RxIRQ(){

  while(MasterUART->readable()){
    char theChar=(char) MasterUART->getc();
    printf("%X\n",theChar);
    // rxBuf.push_back(theChar);
    if (_STATE==ENUMURATION){
      if(theChar==0x59){
          maTh.flags_set(1);
        _STATE=ENUMURATION_PENDING;
      }
      else{
        printf("Got unexpected response during enum %X\n",theChar);
        _STATE=STANDBY;
      }
    }
    else if (_STATE==ENUMURATION_PENDING){
      if(theChar==0x61){
         maTh.flags_set(1);
        _STATE=ENUMURATION;
      }
      else{
        printf("Got unexpected response during enum %X\n",theChar);
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
