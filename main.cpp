#include "MAFW.h"

#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"
#include "stats.hpp"
// #define SYSSTATE

DigitalOut led1(LED1);
DigitalOut led3(LED3);

// AnalogIn   ch0(PA_0);
// AnalogIn   ch1(PA_1);
// AnalogIn   ch2(PA_2);
// AnalogIn   ch3(PA_3);
//
// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);

EventQueue *queue = mbed_event_queue();
serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
#ifdef TERMDEBUG
RawSerial debugPort(PB_6,PB_7,115200);
#endif
Timer t;
Ticker articulator;

std::string Inarticulate::buffer; // initialize buffer for all inarticulate classes

int tik,tok;
void parseCommand(string);
Machine MA;

#ifdef SYSSTATE
SystemReport sys_state( 2000 /* Loop delay time in ms */);
#endif

void articulate();

void flip(){
  led3=!led3;
}


int main()
{
    initStats();
    #ifdef TERMDEBUG
    theTerm.setDebug(&debugPort);
    #endif
    t.start();
    int count=0;

    Meter meter0(&theTerm,0, PC_14, PC_13);
    Stage stage0(&theTerm,0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );
    stage0.armStage();

    theTerm.printf("hello world\n");
    wait(.05);
    #ifdef TERMDEBUG
    theTerm.printDebug("Hello debug");
    #endif
    theTerm.printf("hello world2\n");
    theTerm.attachParser(parseCommand);
    articulator.attach(articulate,0.1);
    statsTicker.attach(updateCPUStats,0.5);
    queue->dispatch_forever();
    while (true) { // polling loop yoooo
        wait(.2); // static delay
        led1=!led1; // life LED

    }
}
void articulate(){
  flip();
  theTerm.printf(Inarticulate::getBuffer().c_str());
}

void parseCommand(string cmd){
  #ifdef TERMDEBUG
  theTerm.printDebug(cmd);
  #endif
  if (strInStr("STATE",cmd)){
    theTerm.printf(MA.report().c_str());
  }

  if (strInStr("CPU",cmd)){
    reportCPUStats();
  }
  if (strInStr("SYS",cmd)){
    reportSystemStats();
  }
  if (strInStr("THREAD",cmd)){
    reportThreadStats();
  }

  if (strInStr("CHARGING",cmd)){
    theTerm.printf("charging cmd\n");
    MA.charging();
  }
  if (strInStr("STANDBY",cmd)){
    theTerm.printf("standby cmd\n");
    MA.standby();
  }
  if (strInStr("LOADED",cmd)){
    theTerm.printf("loaded cmd\n");
    MA.loaded();
  }
  if (strInStr("DISCHARGE",cmd)){
    theTerm.printf("discharge cmd\n");
    MA.discharge();
  }
  if (strInStr("IDLE",cmd)){
    theTerm.printf("idle cmd\n");
    MA.idle();
  }
}
