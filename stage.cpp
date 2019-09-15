#include "stage.hpp"


Stage::Stage(PinName output,PinName voltage, PinName current){
  // theTerm=inTerm;

  voltageADC = new AnalogIn(voltage);
  currentADC = new AnalogIn(current);
  FETgate = new DigitalOut(output);
  printf("initialized stage\n");
}
void Stage::assignTrigger(Trigger* in_trigger){
  trigger=in_trigger;
  trigger->assignPos(callback(this,&Stage::driveHigh)); // tie to triggger edges
  trigger->assignNeg(callback(this,&Stage::driveLow));
  printf("trigger assigned stage\n");
}
void Stage::assignMeter(Meter* in_meter){
  meter=in_meter;
  printf("meter assigned\n");
}

float Stage::getCurrent(){
  return currentADC->read();
}
float Stage::getVoltage(){
  return voltageADC->read();
}

void Stage::driveHigh(){
  FETgate->write(1);
}
void Stage::driveLow(){
  FETgate->write(0);
}
void Stage::printStats(){
  // printf("Stage\tDelay\tWidth\tArm\tState\tV\tI\n");
  //TODO: Fix this later
  // printf("%d \t%d \t%d \t %d \t %d \t%.2f \t%.2f\t%.2f\n",id,posDelay,pulseWidth,armed,state,getVoltage(),getCurrent(),meter->getVelocity());
}
