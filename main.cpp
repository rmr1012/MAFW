// #include "MAFW.h"
//
// #include "machine.hpp"
// #include "stage.hpp"
// #include "terminal.hpp"
// #include "utilities.hpp"
// #include "stats.hpp"
// // #define SYSSTATE
//
// DigitalOut led1(LED1);
// DigitalOut led3(LED3);
//
// DigitalOut pulseO(LED2);
// Timeout TOOneshot;
// void pulseLow(){
//   pulseO.write(0);
// }
//
// // InterruptIn armSwitch(PB_10);
// // InterruptIn loadMeter(PC_15);
//
// // EventQueue *queue = mbed_event_queue();
// Machine MA(D1,D0,A1,A0);
// serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
// // Ticker articulator;
//
// // std::string Inarticulate::buffer; // initialize buffer for all inarticulate classes
//
// void parseCommand(string);
//
//
//
//
// int main()
// {
//     // MA.MasterUART->printf("wtfffbro\n");
//     printf("hello world\n");
//     initStats();
//
//     MA.startEnumeration();
//
//
//     wait(.05);
//     theTerm.attachParser(parseCommand);
//
//     led3=0;
//     led1=1;
//     printf("ready to work\nterm$ ");
//     while (true) { // polling loop yoooo
//         wait(.2); // static delay
//         led1=!led1; // life LED
//         led3=!led3; // life LED
//
//
//     }
// }
//
// void parseCommand(string cmd){
//
//   if (strInStr("STAGES",cmd)){ // print all stages
//     theTerm.printf("WTF\n");
//   }
//
// }

#include "MAFW.h"
RawSerial MasterUART(USBTX,USBRX,115200);
RawSerial SlaveUART(D1,D0,500000);

void forwardMS(){
  SlaveUART.putc(MasterUART.getc());
}
void forwardSM(){
  MasterUART.printf("0x%X ",SlaveUART.getc());
}
int main(){
  MasterUART.attach(forwardMS);
  SlaveUART.attach(forwardSM);
  while(1){
  }
}
