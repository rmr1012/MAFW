#include "meter.hpp"

Meter::Meter(PinName in_cp1, PinName in_cp2){
  if (in_cp1==in_cp2){
    cp = new InterruptIn(in_cp1);
    timer.start();
    cp->rise(callback(this,&Meter::cpuISR));
    cp->fall(callback(this,&Meter::cpdISR));
    printf("single LED meter initialized\n");
  }
  else{
    cp = new InterruptIn(in_cp1);
    cp2 = new InterruptIn(in_cp2);
    timer.start();
    cp->rise(callback(this,&Meter::cpuISR));
    cp2->rise(callback(this,&Meter::cpdISR));
    printf("dual LED meter initialized\n");
    mode=METER_DUAL_LED_MODE;
  }
}

float Meter::getVelocity(){
  if(mode==METER_SINGLE_LED_MODE)
    return (float)19050.0/(cpd-cpu);// deug return just 2ud width 19050=.01905m*10^6(u)
  else{
    return (float)7112.0/(cpd-cpu);// dual mode 7112 = 0.28in
  }
  // return 1/(((cp1d-cp1u)+(cp1d-cp1u))/2);
}

void Meter::cpuISR(){
  if(armed){
    // printf("armed and triggerd U \n ");
    cpu=timer.read_us();
  }
}
void Meter::cpdISR(){
  if(armed){
    cpd=timer.read_us();
    resultReady=true;
    // armed=false;
  }
}
