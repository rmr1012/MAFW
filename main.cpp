#include "MAFW.h"

#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"
#include "stats.hpp"
// #define SYSSTATE

DigitalOut led1(LED1);
DigitalOut led3(LED3);

DigitalOut pulseO(PC_8);
Timeout TOOneshot;
void pulseLow(){
  pulseO.write(0);
}

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
void parseStage(string inStr);
void parseOneshot(string inStr);

void pollADC();
Machine MA;
Meter meter0(&theTerm,0, PC_14, PC_13);
Stage stage0(&theTerm,0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );

#ifdef SYSSTATE
SystemReport sys_state( 2000 /* Loop delay time in ms */);
#endif

void articulate();

void flip(){
  led3=!led3;
}


int main()
{
    pulseO.write(1);
    initStats();
    #ifdef TERMDEBUG
    theTerm.setDebug(&debugPort);
    #endif
    t.start();
    int count=0;


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

    pulseO.write(0);

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
  if (strInStr("STAGES",cmd)){ // print all stages
    printf("Stage\tDelay\tWidth\tArm\tState\tV\tI\tSpeed\n");
    stage0.printStats();
  }
  else if(strInStr("STAGE ",cmd)){ // print all stages
    parseStage(cmd);
  }
  else if(strInStr("ONESHOT ",cmd)){ // print oneshot
    printf("Setting up oneshot\n");
    parseOneshot(cmd);
  }
  else if (strInStr("STATE",cmd)){
    theTerm.printf(MA.report().c_str());
  }

  else if (strInStr("CPU",cmd)){
    reportCPUStats();
  }
  else if (strInStr("SYS",cmd)){
    reportSystemStats();
  }
  else if (strInStr("THREAD",cmd)){
    reportThreadStats();
  }

  else if (strInStr("CHARGING",cmd)){
    theTerm.printf("charging cmd\n");
    MA.charging();
  }
  else if (strInStr("STANDBY",cmd)){
    theTerm.printf("standby cmd\n");
    MA.standby();
  }
  else if (strInStr("LOADED",cmd)){
    theTerm.printf("loaded cmd\n");
    MA.loaded();
  }
  else if (strInStr("DISCHARGE",cmd)){
    theTerm.printf("discharge cmd\n");
    MA.discharge();
  }
  else if (strInStr("IDLE",cmd)){
    theTerm.printf("idle cmd\n");
    MA.idle();
  }
  else{
    printf("Command not found\n");
  }
}

void parseStage(string inStr){
  // theTerm.printDebug((char*)inStr.c_str());
  regex IORre("STAGE ([0-9]) (DELAY|WIDTH) ([0-9]{1,6})");
  smatch match;
  if(regex_search(inStr,match,IORre)){ // check syntax
      // try {
      // theTerm.printDebug("IO read syntax matched!\n");
      int stageNum=stoi(match[1].str());
      string prop = match[2].str();
      int val=stoi(match[3].str());
      printf("Setting stage %d %s to %d\n",stageNum,prop.c_str(),val);
      if (prop == "DELAY"){
        stage0.setDelay(val);
      }
      else{
        stage0.setWidth(val);
      }
  }
  else{
    printf("Syntax error, check syntax or type: help\n");
  }
}

void parseOneshot(string inStr){
  // theTerm.printDebug((char*)inStr.c_str());
  regex IORre("ONESHOT ([0-9]*)");// pulse width(us)
  smatch match;
  if(regex_search(inStr,match,IORre)){ // check syntax
      // try {
      // theTerm.printDebug("IO read syntax matched!\n");
      int pulseWidth=stoi(match[1].str());
      pulseO.write(1);
      TOOneshot.attach_us(callback(pulseLow),pulseWidth);
  }
  else{
    printf("Syntax error, check syntax or type: help\n");
  }
}
