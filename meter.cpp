#include "meter.hpp"

Meter::Meter (int in_id, PinName in_cp1, PinName in_cp2){
  id=in_id;
  cp1 = new InterruptIn(in_cp1);
  cp2 = new InterruptIn(in_cp2);
  void (Meter::* u1ptr) (void) = &Meter::cp1uISR;
  timer.start();
  cp1->rise(callback(this,&Meter::cp1uISR));
  cp1->fall(callback(this,&Meter::cp1dISR));
  cp2->rise(callback(this,&Meter::cp2uISR));
  cp2->fall(callback(this,&Meter::cp2dISR));
  printf("meter %i initialized\n",id);
}
int Meter::getID(){
  return id;
}
float Meter::getVelocity(){
  return cp2d-cp2u;// deug return just 2ud width
  // return 1/(((cp1d-cp1u)+(cp1d-cp1u))/2);
}
void Meter::assignTrigger(Callback<void()> func){
  triggerFunc=func;
}
void Meter::cp1uISR(){
  cp1u=timer.read_us();
  printf("cp1uISR\n");
}
void Meter::cp1dISR(){
  cp1d=timer.read_us();
  printf("cp1dISR\n");
}
void Meter::cp2uISR(){
  cp2u=timer.read_us();
  printf("cp2uISR\n");
}
void Meter::cp2dISR(){
  cp2d=timer.read_us();
  printf("cp2dISR\n");
  triggerFunc();
}
