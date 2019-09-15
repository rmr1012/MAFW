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
Machine::Machine(){
    current = new IDLE(); // initial state
};


void Machine::appendStage(PinName triggerPin, PinName outputPin, PinName meterPin, PinName voltagePin, PinName currentPin){
  Meter* meter=new Meter(meterPin);
  Trigger* trigger = new Trigger(triggerPin);

  Stage* stage=new Stage(outputPin, voltagePin, currentPin);
  stage->assignTrigger(trigger);
  stage->assignMeter(meter);

  meters.push_back(meter);
  triggers.push_back(trigger);
  stages.push_back(stage);
}
Stage* Machine::getStage(int index){
  return stages[index];
}
Meter* Machine::getMeter(int index){
  return meters[index];
}
Trigger* Machine::getTrigger(int index){
  return triggers[index];
}
void Machine::reportStages(){
  printf("   \tStage\tDelay\tWidth\tV\tI\tSpeed\n");
  for(std::vector<int>::size_type i = 0; i != stages.size(); i++) {
    printf("%i\t",i);
    stages[i]->printStats();
  }
}

string Machine::report(){
  string report="==== STATE MACHINE ====\n";
  report += current->report();
  return report;
}



void Machine::setCurrent(State *s){
    current = s;
}
void Machine::idle(){current->idle(this);}
void Machine::charging(){current->charging(this);}
void Machine::standby(){current->standby(this);}
void Machine::loaded(){current->loaded(this);}
void Machine::discharge(){current->discharge(this);}



void State::idle(Machine *m){}
void State::standby(Machine *m){}
void State::loaded(Machine *m){}
void State::discharge(Machine *m){}
void State::charging(Machine *m){}
string State::report(){return "nothing to report\n";}

void IDLE::charging(Machine *m){
    printf("   going from IDLE to CHARGING\n");
    m->setCurrent(new CHARGING());
    delete this;
}
string IDLE::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}
void CHARGING::standby(Machine *m){
    printf("   going from CHARGING to STANDBY\n");
    m->setCurrent(new STANDBY());
    delete this;
}
string CHARGING::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}
void STANDBY::loaded(Machine *m){
    printf("   going from STANDBY to LOADED\n");
    m->setCurrent(new LOADED());
    delete this;
}
string STANDBY::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}
void LOADED::discharge(Machine *m){
    printf("   going from LOADED to DISCHARGE\n");
    m->setCurrent(new DISCHARGE());
    delete this;
}
string LOADED::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}
void DISCHARGE::idle(Machine *m){
    printf("   going from DISCHARGE to IDLE\n");
    m->setCurrent(new IDLE());
    delete this;
}
string DISCHARGE::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}
