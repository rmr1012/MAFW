#include "meter.hpp"

Meter::Meter(PinName in_cp1){
  cp = new InterruptIn(in_cp1);
  timer.start();
  cp->rise(callback(this,&Meter::cpuISR));
  cp->fall(callback(this,&Meter::cpdISR));
  printf("meter initialized\n");
}

float Meter::getVelocity(){
  return (float)19050.0/(cpu-cpd);// deug return just 2ud width 19050=.01905m*10^6(u)
  // return 1/(((cp1d-cp1u)+(cp1d-cp1u))/2);
}

void Meter::cpuISR(){
  cpu=timer.read_us();
  resultReady=true;
}
void Meter::cpdISR(){
  cpd=timer.read_us();

}
