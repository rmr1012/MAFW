#include "MAFW.h"
// #include "stats_report.h"
#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"

DigitalOut led1(LED1);
DigitalOut led3(LED3);

// AnalogIn   ch0(PA_0);
// AnalogIn   ch1(PA_1);
// AnalogIn   ch2(PA_2);
// AnalogIn   ch3(PA_3);
//
// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);
// Serial pc(PB_6,PB_7);
// Serial debugUART(PTC15,PTC14);
EventQueue *queue = mbed_event_queue();
serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
RawSerial debugPort(PB_6,PB_7,115200);
Timer t;
Ticker articulator;

std::string Inarticulate::buffer; // initialize buffer for all inarticulate classes

int tik,tok;
void parseCommand(string);
Machine MA;
// SystemReport sys_state( 2000 /* Loop delay time in ms */);

// Create a queue that can hold a maximum of 32 events
// static EventQueue queue(32 * EVENTS_EVENT_SIZE);
// // Create a thread that'll run the event queue's dispatch function
// static Thread t;
void articulate();
void flip(){
  led3=!led3;
}

int main()
{
    // SystemReport sys_state(2000 /* Loop delay time in ms */);
    theTerm.setDebug(&debugPort);
    t.start();
    int count=0;

    Meter meter0(&theTerm,0, PC_14, PC_13);
    Stage stage0(&theTerm,0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );
    stage0.armStage();

    theTerm.printf("hello world\n");
    wait(.05);
    theTerm.printDebug("Hello debug");
    theTerm.printf("hello world2\n");
    theTerm.attachParser(parseCommand);
    // theTerm.attach(callback(&flip));
    articulator.attach(articulate,0.1);
    queue->dispatch_forever();
    while (true) { // polling loop yoooo
        wait(.2); // static delay
        // wdt_timer.start(WDT_LENGTH); // reset WDT timer
        led1=!led1; // life LED
        // if(theTerm.commandReady){
        //   theTerm.printDebug("calling parsing command...\n");
        //   theTerm.commandReady=false;
        //   parseCommand(theTerm.commandBuffer);
        //   theTerm.commandBuffer="";
        //   theTerm.printf("\nterm$ ");
        // }
        // if(1)//Inarticulate::isReady()
        // {
        //   // theTerm.printf("\ncool stuff$ ");
        //   theTerm.printf(Inarticulate::getBuffer().c_str());
        // }
    }
}
void articulate(){
  flip();
  theTerm.printf(Inarticulate::getBuffer().c_str());
}
void parseCommand(string cmd){
  theTerm.printDebug(cmd);
  if (strInStr("STATE",cmd)){
    theTerm.printf(MA.report().c_str());
  }
  if (strInStr("SYS",cmd)){
    theTerm.printf("reporting sys_state\n");
    // queue->call(&sys_state,&SystemReport::report_state);
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
