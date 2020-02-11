#include "MAFW.h"
#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"
#include "stats.hpp"
// #define SYSSTATE

DigitalOut led1(LED1);
DigitalOut led3(LED3);

DigitalOut fakeTrigger(A5);

DigitalOut pulseO(LED2);
Timeout TOOneshot;
void pulseLow(){
  pulseO.write(0);
}

// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);

// EventQueue *queue = mbed_event_queue();

serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
// Ticker articulator;

// std::string Inarticulate::buffer; // initialize buffer for all inarticulate classes

void parseCommand(string);


Machine MA(D1,D0,A1,A0);

int main()
{
    // MA.MasterUART->printf("wtfffbro\n");
    printf("hello world\n");
    initStats();

    // MA.startEnumeration();
    MA.enumerateStages();
    wait(.1);
    MA.armMachine();
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

  if (strInStr("STAGES",cmd)){ // print all stages
    theTerm.printf("WTF\n");
  }
  else if (strInStr("RESET",cmd)){ // print all stages
    theTerm.printf("Resetting Stages\n");
    MA.resetStages();
  }
  else if (strInStr("ENUM",cmd)){ // print all stages
    theTerm.printf("reEnumerating Stages\n");
    MA.enumerateStages();
  }
  else if (strInStr("STREAM",cmd)){ // print all stages
    theTerm.printf("Streaming from Stages\n");
    MA.streamStages();
  }
  else if (strInStr("METER ",cmd)){ // print all stages
    int stage = stoi(cmd.substr(6));
    theTerm.printf("metering from Stages\n");
    MA.pollMeters();
  }
  else if (strInStr("PING ",cmd)){ // print all stages
    int stage = stoi(cmd.substr(6));
    theTerm.printf("pinging from Stages\n");
    // MA.pingStage(stage);
  }
  else if (strInStr("ARM",cmd)){ // print all stages
    MA.armMachine();
    theTerm.printf("arming Stages\n");
    // MA.pingStage(stage);
  }
  else if (strInStr("FIRE ",cmd)){ // print all stages
    int dt = stoi(cmd.substr(5));
    fakeTrigger=1;
    theTerm.printf("firing stage 1\n");
    MA.fire();
    wait(0.001*dt);
    fakeTrigger=0;
  }
  else if (strInStr("READREG ",cmd)){ // print all stages
    uint8_t stage = stoi(cmd.substr(8,9));
    uint8_t reg = stoi(cmd.substr(10));
    theTerm.printf("Reading stage %i reg 0x%X\n",stage,reg);
    uint8_t data=MA.readReg(stage,reg);
    printf("data is 0x%X\n",data);
  }
  else if (strInStr("WRITEREG ",cmd)){ // print all stages
    uint8_t stage = stoi(cmd.substr(9,10));
    uint8_t reg = stoi(cmd.substr(11,12));
    uint8_t data = stoi(cmd.substr(13));
    theTerm.printf("Writing stage %i reg 0x%X data 0x%X\n",stage,reg,data);
    MA.writeReg(stage,reg,data);
  }


}

// #include "MAFW.h"
// RawSerial MasterUART(USBTX,USBRX,115200);
// RawSerial SlaveUART(D1,D0,500000);
//
// void forwardMS(){
//   SlaveUART.putc(MasterUART.getc());
// }
// void forwardSM(){
//   MasterUART.printf("0x%X ",SlaveUART.getc());
// }
// int main(){
//   MasterUART.attach(forwardMS);
//   SlaveUART.attach(forwardSM);
//   while(1){
//   }
// }
