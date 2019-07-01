//
//  machine.cpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#include <stdio.h>
#include "machine.hpp"
string Inarticulate::buffer;
Machine::Machine(){
    current = new IDLE(); // initial state
};
void Machine::setCurrent(State *s){
    current = s;
}
void Machine::idle(){current->idle(this);}
void Machine::charging(){current->charging(this);}
void Machine::standby(){current->standby(this);}
void Machine::loaded(){current->loaded(this);}
void Machine::discharge(){current->discharge(this);}

void Machine::appendMeter(Meter* newMeter){
  meters.push_back(newMeter);
}
void Machine::appendStage(Stage* newStage){
  stages.push_back(newStage);
}
string Machine::report(){
  string report="==== STATE MACHINE ====\n";
  report += current->report();
  return report;
}

void State::idle(Machine *m){}
void State::standby(Machine *m){}
void State::loaded(Machine *m){}
void State::discharge(Machine *m){}
void State::charging(Machine *m){}
string State::report(){return "nothing to report\n";}

void IDLE::charging(Machine *m){
    bprintf("   going from IDLE to CHARGING");
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
    bprintf("   going from CHARGING to STANDBY");
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
    bprintf("   going from STANDBY to LOADED");
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
    bprintf("   going from LOADED to DISCHARGE");
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
    bprintf("   going from DISCHARGE to IDLE");
    m->setCurrent(new IDLE());
    delete this;
}
string DISCHARGE::report(){
  string report= "Current State: ";
  report += name;
  report += "\n";

  return report;
}