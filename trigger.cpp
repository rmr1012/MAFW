#include "trigger.hpp"

Trigger::Trigger(PinName triggerPri, PinName triggerSec){
  triggerI= new InterruptIn(triggerPri);
  triggerI->rise(callback(this,&Trigger::posEdgeISR));
  if (triggerPri==triggerSec){
    triggerI->fall(callback(this,&Trigger::negEdgeISR));
  }
  else{// stage 0 special case
    triggerI2= new InterruptIn(triggerSec);
    triggerI2->fall(callback(this,&Trigger::negEdgeISR));
  }
}

void Trigger::posEdgeISR(){
  safetyTO.attach_us(callback(this,&Trigger::negEdgeSignalWrap), 1000*safeTime);
  if(state==TRIGGER_ARMED){
    state=TRIGGER_WORKING;
    if (posDelay>80){
      outPosEdgeTO.attach_us(callback(this,&Trigger::posEdgeSignalWrap),posDelay-80);
    }
    else{
      wait_us(posDelay);// janky fix for timing inperfections
      posEdgeSignal();
    }
    if (posDelayP>80){
      outPosEdgeTOP.attach_us(callback(this,&Trigger::posEdgeSignalWrapP),posDelayP-80);
    }
    else{
      wait_us(posDelayP);// janky fix for timing inperfections
      posEdgeSignalP();
    }

    if (mode){// delay only pulse width
      outNegEdgeTO.attach_us(callback(this,&Trigger::negEdgeSignalWrap), posDelay+pulseWidth);
    }
  }
  printf("exiting posEdgeISR\n");
  // else don't time anything out, wait for neg edge
}
void Trigger::negEdgeISR(){
  if(!mode){// if trigger + delay neg edge mode
    if(negDelay){
      outNegEdgeTO.attach_us(callback(this,&Trigger::negEdgeSignalWrap), negDelay);
    }
    else{
      negEdgeSignal();
      state=TRIGGER_SPENT;
      triggerSpentCallback();
    }
  }
  printf("exiting negEdgeISR\n");
}

void Trigger::armTrigger(){
  state=TRIGGER_ARMED;
  printf("trigger armed\n");
}

void Trigger::assignSpent(Callback<void()> func){
  triggerSpentCallback=func;
}

void Trigger::assignPos(Callback<void()> func){
  posEdgeSignal=func;
}
void Trigger::assignNeg(Callback<void()> func){
  negEdgeSignal=func;
}
void Trigger::assignPosP(Callback<void()> func){
  posEdgeSignalP=func;
}
void Trigger::assignNegP(Callback<void()> func){
  negEdgeSignalP=func;
}
