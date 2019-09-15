#include "trigger.hpp"

Trigger::Trigger(PinName triggerPin,bool first){
  triggerI= new InterruptIn(triggerPin);
  triggerI->rise(callback(this,&Trigger::posEdgeISR));
  triggerI->fall(callback(this,&Trigger::negEdgeISR));
}

void Trigger::posEdgeISR(){
  if (posDelay>80){
    outPosEdgeTO.attach_us(callback(this,&Trigger::posEdgeSignalWrap),posDelay-80);
  }
  else{
    wait_us(posDelay);// janky fix for timing inperfections
    posEdgeSignal();
  }

  if (mode){// delay only pulse width
    outNegEdgeTO.attach_us(callback(this,&Trigger::negEdgeSignalWrap), posDelay+pulseWidth);
  }
  // else don't time anything out, wait for neg edge
}
void Trigger::negEdgeISR(){
  if(!mode){// if trigger + delay neg edge mode
    if(negDelay){
      outNegEdgeTO.attach_us(callback(this,&Trigger::negEdgeSignalWrap), negDelay);
    }
    else{
      negEdgeSignal();
    }
  }
}

void Trigger::assignPos(Callback<void()> func){
  posEdgeSignal=func;
}
void Trigger::assignNeg(Callback<void()> func){
  negEdgeSignal=func;
}

void Trigger::setMode(bool in_mode){
  mode=in_mode;
}
void Trigger::setPosDelay(int delay){
  posDelay=delay;
}
void Trigger::setNegDelay(int delay){
  negDelay=delay;
}
void Trigger::setPulseWidth(int width){
  pulseWidth=width;
}
