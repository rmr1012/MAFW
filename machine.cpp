//
//  machine.cpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#include <stdio.h>

#include "machine.hpp"

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


void IDLE::charging(Machine *m){
    printf("   going from IDLE to CHARGINGn");
    m->setCurrent(new CHARGING());
    delete this;
}
void CHARGING::standby(Machine *m){
    printf("   going from CHARGING to STANDBYn");
    m->setCurrent(new STANDBY());
    delete this;
}

void STANDBY::loaded(Machine *m){
    printf("   going from STANDBY to LOADEDn");
    m->setCurrent(new LOADED());
    delete this;
}
void LOADED::discharge(Machine *m){
    printf("   going from LOADED to DISCHARGEn");
    m->setCurrent(new DISCHARGE());
    delete this;
}
void DISCHARGE::idle(Machine *m){
    printf("   going from DISCHARGE to IDLEn");
    m->setCurrent(new IDLE());
    delete this;
}
