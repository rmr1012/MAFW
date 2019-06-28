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
void State::idle(Machine *m){}
void State::standby(Machine *m){}
void State::loaded(Machine *m){}
void State::discharge(Machine *m){}
void State::charging(Machine *m){}

void IDLE::charging(Machine *m){
    bprintf("   going from IDLE to CHARGING");
    m->setCurrent(new CHARGING());
    delete this;
}
void CHARGING::standby(Machine *m){
    bprintf("   going from CHARGING to STANDBY");
    m->setCurrent(new STANDBY());
    delete this;
}

void STANDBY::loaded(Machine *m){
    bprintf("   going from STANDBY to LOADED");
    m->setCurrent(new LOADED());
    delete this;
}
void LOADED::discharge(Machine *m){
    bprintf("   going from LOADED to DISCHARGE");
    m->setCurrent(new DISCHARGE());
    delete this;
}
void DISCHARGE::idle(Machine *m){
    bprintf("   going from DISCHARGE to IDLE");
    m->setCurrent(new IDLE());
    delete this;
}
