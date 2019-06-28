#include "stage.hpp"


Stage::Stage(serialTerminal* theTerm, int in_id, Meter* pre_meter, float in_delay, PinName voltage, PinName current,PinName out_trigger, PinName in_trigger):theTerm(theTerm){
  // theTerm=inTerm;
  id=in_id;
  state=0;// default state to idle
  delay=in_delay;
  meter=pre_meter;

  voltageADC = new AnalogIn(voltage);
  currentADC = new AnalogIn(current);
  triggerO = new DigitalOut(out_trigger);
  if (in_trigger != NC){
    triggerI = new InterruptIn(in_trigger);
    triggerI->rise(callback(this,&Stage::triggerISR));
    meter->assignTrigger(callback(this,&Stage::donothing));
    theTerm->printf("hand trig assigned\n");
  }
  else{
    meter->assignTrigger(callback(this,&Stage::triggerISR));
    theTerm->printf("auto meter trig assigned\n");
  }
  theTerm->printf("initializing stage %i\n",id);
}

int Stage::getID(){
  return id;
}
int Stage::getState(){
  return state;
}
float Stage::getCurrent(){
  return currentADC->read();
}
float Stage::getVoltage(){
  return voltageADC->read();
}
void Stage::armStage(){
  armed=true;
}
void Stage::driveHigh(){
  triggerO->write(1);
}
void Stage::driveLow(){
  triggerO->write(0);
}
void Stage::discharge(){
  if(armed){
    theTerm->printf("in discharge\n");
    theTerm->printf("elapsed: %i  us\n",meter->cp2d-meter->cp2u);
    outPosEdge.attach_us(callback(this,&Stage::driveHigh),100);
    outNegEdge.attach_us(callback(this,&Stage::driveLow), 600);
  }
}
void Stage::triggerISR(){
  theTerm->printf("in trigger ISR\n");
  discharge();
}

void Stage::donothing(){}
