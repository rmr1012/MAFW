#include "MAFW.h"
// #include "stats_report.h"
#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"


DigitalOut led1(LED2);


// AnalogIn   ch0(PA_0);
// AnalogIn   ch1(PA_1);
// AnalogIn   ch2(PA_2);
// AnalogIn   ch3(PA_3);
//
// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);
// Serial pc(PB_6,PB_7);
// Serial debugUART(PTC15,PTC14);
serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
Timer t;


int tik,tok;
void parseCommand(string);
EventQueue queue;
Machine MA;

int main()
{

    t.start();
    // SystemReport sys_state( SLEEP_TIME * PRINT_AFTER_N_LOOPS /* Loop delay time in ms */);
    int count=0;

    Meter meter0(&theTerm,0, PC_14, PC_13);
    Stage stage0(&theTerm,0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );
    stage0.armStage();

    theTerm.printf("hello world\n");

    while (true) { // polling loop yoooo
        wait(.2); // static delay
        // wdt_timer.start(WDT_LENGTH); // reset WDT timer
        led1=!led1; // life LED
        if(theTerm.commandReady){
          theTerm.printDebug("calling parsing command...\n");
          theTerm.commandReady=false;
          parseCommand(theTerm.commandBuffer);
          theTerm.commandBuffer="";
          theTerm.printf("\nterm$ ");
        }
        if(1)//Inarticulate::isReady()
        {
          // theTerm.printf("\ncool stuff$ ");
          theTerm.printf(Inarticulate::getBuffer().c_str());
        }
    }
}
void parseCommand(string cmd){
  if (strInStr("1",cmd)){
    theTerm.printf("charging cmd");
    MA.charging();
  }
  if (strInStr("2",cmd)){
    theTerm.printf("standby cmd");
    MA.standby();
  }
  if (strInStr("3",cmd)){
    theTerm.printf("loaded cmd");
    MA.loaded();
  }
  if (strInStr("4",cmd)){
    theTerm.printf("discharge cmd");
    MA.discharge();
  }
  if (strInStr("5",cmd)){
    theTerm.printf("idle cmd");
    MA.idle();
  }
}
// #include "mbed.h"
//
// DigitalOut led1(LED1);
// DigitalOut led2(LED2);
//
// RawSerial pc(USBTX, USBRX);
//
// void callback_ex() {
//     // Note: you need to actually read from the serial to clear the RX interrupt
//     char poop = pc.getc();
//     pc.putc('a');
//     pc.printf("yoni\n" );
//     led1 = !led1;
// }
//
// int main() {
//     // printf("lingam\n" );
//     pc.attach(&callback_ex);
//
//     while (1) {
//         led2 = !led2;
//         wait(0.5);
//     }
// }
