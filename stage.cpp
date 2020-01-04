#include "stage.hpp"


Stage::Stage(PinName output,PinName outputP,PinName voltage, PinName current){
  // theTerm=inTerm;

  voltageADC = new AnalogIn(voltage);
  currentADC = new AnalogIn(current);
  FETgate = new DigitalOut(output);
  if (output != outputP){
    FETgateP = new DigitalOut(outputP);
  }
  ADCThread.start(callback(this,&Stage::record));
  printf("initialized stage\n");
}
void Stage::assignTrigger(Trigger* in_trigger){
  trigger=in_trigger;
  trigger->assignPos(callback(this,&Stage::driveHigh)); // tie to triggger edges
  trigger->assignNeg(callback(this,&Stage::driveLow));
  // if (output != outputP){
    trigger->assignPosP(callback(this,&Stage::driveHighP)); // tie to triggger edges
    trigger->assignNegP(callback(this,&Stage::driveLowP));
  // }
  printf("trigger assigned stage\n");
}
void Stage::assignMeter(Meter* in_meter){
  meter=in_meter;
  printf("meter assigned\n");
}
void Stage::record(){
  Timer perfTimer;
  int tic,toc,samplePeriod;
  perfTimer.start();
  ADCThreadID = osThreadGetId();
  printf("ADC Thread Started\n");
  for(;;){
    osSignalWait(0x01, osWaitForever);
    tic=perfTimer.read_us();
    for(int i=0;i<=100;i++){
      currentVals[i]=getCurrent();
      voltageVals[i]=getVoltage();//110us Sample time ASAP
      // wait_us(recordInterval);
    }
    toc=perfTimer.read_us();
    samplePeriod=toc-tic;

    jules=0;

    for(int i=0;i<=100;i++){
      if(currentVals[i]>=0.01)
        jules+=currentVals[i]*voltageVals[i];
      // printf("%.2f\t%.2f\n",currentVals[i],voltageVals[i]);

    }
    jules=jules*samplePeriod*0.00000001*ADC2AMP*ADC2VOLT;//x 110us

    while(!meter->resultReady){wait_us(75);}
    wait_us(1000);// temporary before result ready flag is up
    if(abs(meter->cpd-meter->cpu)>50){//janky filter to kill the outliers
      kinetic=0.5*2.9*(meter->getVelocity())*(meter->getVelocity())/1000;
      printf("%i %i %i\n",meter->cpd-meter->cpu,meter->cpd,meter->cpu);
      printf("%.3f m/s %.4f%% @ %.6f J / %.3f J\n",meter->getVelocity(),(kinetic/jules)*100,kinetic,jules);
    }
  }
}

float Stage::getCurrent(){
  return currentADC->read();
}
float Stage::getVoltage(){
  return voltageADC->read();
}

void Stage::driveHigh(){
  meter->resultReady=false;
  osSignalSet(ADCThreadID,0x01);
  FETgate->write(1);
}
void Stage::driveLow(){
  FETgate->write(0);
  wait_us(100);//janky alart
  meter->arm();
}

void Stage::driveHighP(){
  FETgateP->write(1);
}
void Stage::driveLowP(){
  FETgateP->write(0);
}
void Stage::printStats(){
  // printf("Stage\tDelay\tWidth\tArm\tState\tV\tI\n");
  //TODO: Fix this later
  printf("$$$\t%d \t%d \t%.2f\t%.2f\t%.2f\n",trigger->posDelay,trigger->pulseWidth,getVoltage(),getCurrent(),meter->getVelocity());

}
