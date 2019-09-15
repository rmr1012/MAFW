#include "MAFW.h"

#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"
#include "stats.hpp"
// #define SYSSTATE

DigitalOut led1(LED1);
DigitalOut led3(LED3);

DigitalOut pulseO(D15);
Timeout TOOneshot;
void pulseLow(){
  pulseO.write(0);
}

// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);

// EventQueue *queue = mbed_event_queue();
serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx

Timer t;
// Ticker articulator;

// std::string Inarticulate::buffer; // initialize buffer for all inarticulate classes

int tik,tok;
void parseCommand(string);
void parseStage(string inStr);
void parseOneshot(string inStr);

Machine MA;
// Meter meter0(PC_13);
// Stage stage0(&theTerm,0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );
// Stage stage0(&theTerm,0 , &meter0,  A0 , A1 , PC_8 );



int main()
{
    initStats();
      //           trig   out meter  V   I
    MA.appendStage(PC_13, D15, D0, A0, A1);


    MA.getTrigger(0)->setMode(1);

    printf("hello world\n");
    wait(.05);
    theTerm.attachParser(parseCommand);


    led3=0;
    led1=1;
    printf("ready to work\nterm$ ");
    while (true) { // polling loop yoooo
        wait(.2); // static delay
        led1=!led1; // life LED
        led3=!led3; // life LED

    }
}

void parseCommand(string cmd){
  #ifdef TERMDEBUG
  theTerm.printDebug(cmd);
  #endif
  if (strInStr("STAGES",cmd)){ // print all stages
    MA.reportStages();
  }
  else if(strInStr("STAGE ",cmd)){ // print all stages
    parseStage(cmd);
  }
  else if(strInStr("ONESHOT ",cmd)){ // print oneshot
    printf("Setting up oneshot\n");
    parseOneshot(cmd);
  }
  else if (strInStr("STATE",cmd)){
    printf(MA.report().c_str());
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
    printf("charging cmd\n");
    MA.charging();
  }
  else if (strInStr("STANDBY",cmd)){
    printf("standby cmd\n");
    MA.standby();
  }
  else if (strInStr("LOADED",cmd)){
    printf("loaded cmd\n");
    MA.loaded();
  }
  else if (strInStr("DISCHARGE",cmd)){
    printf("discharge cmd\n");
    MA.discharge();
  }
  else if (strInStr("IDLE",cmd)){
    printf("idle cmd\n");
    MA.idle();
  }
  else{
    printf("Command not found\n");
  }
}

void parseStage(string inStr){
  // theTerm.printDebug((char*)inStr.c_str());
  regex IORre("STAGE ([0-9]) (PDELAY|NDELAY|WIDTH) ([0-9]{1,6})");
  smatch match;
  if(regex_search(inStr,match,IORre)){ // check syntax
      // try {
      // theTerm.printDebug("IO read syntax matched!\n");
      int stageNum=stoi(match[1].str());
      string prop = match[2].str();
      int val=stoi(match[3].str());
      printf("Setting stage %d %s to %d\n",stageNum,prop.c_str(),val);
      if (prop == "PDELAY"){
        MA.getStage(stageNum)->trigger->setPosDelay(val);
      }
      else if (prop == "NDELAY"){
        MA.getStage(stageNum)->trigger->setNegDelay(val);
      }
      else if (prop == "WIDTH"){
        MA.getStage(stageNum)->trigger->setPulseWidth(val);
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
