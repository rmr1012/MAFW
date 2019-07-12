#include "MAFW.h"

#include "machine.hpp"
#include "stage.hpp"
#include "terminal.hpp"
#include "utilities.hpp"
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

mbed_stats_heap_t   heap_stats;
mbed_stats_cpu_t    cpu_stats;
mbed_stats_sys_t    sys_stats;

mbed_stats_thread_t *thread_stats;
uint8_t   thread_count;
uint8_t   max_thread_count=10;



EventQueue *queue = mbed_event_queue();
serialTerminal theTerm(USBTX,USBRX,115200);// tx, rx
#ifdef TERMDEBUG
RawSerial debugPort(PB_6,PB_7,115200);
#endif
Timer t;
Ticker articulator;

Ticker statsTicker;
uint64_t cpu_diff;
void updateCPUStats(){
  static uint64_t prev_idle_time = 0;
  mbed_stats_cpu_get(&cpu_stats);
  cpu_diff = (cpu_stats.idle_time - prev_idle_time);
  prev_idle_time = cpu_stats.idle_time;
}
void reportThreadStats(){
  printf("================ THREAD STATS ===============\r\n");

  // Collect and print running thread stats
  int count = mbed_stats_thread_get_each(thread_stats, max_thread_count);

  for (int i = 0; i < count; i++) {
      printf("ID: 0x%lx \r\n",        thread_stats[i].id);
      printf("Name: %s \r\n",         thread_stats[i].name);
      printf("State: %ld \r\n",       thread_stats[i].state);
      printf("Priority: %ld \r\n",    thread_stats[i].priority);
      printf("Stack Size: %ld \r\n",  thread_stats[i].stack_size);
      printf("Stack Space: %ld \r\n", thread_stats[i].stack_space);
  }
}
void reportCPUStats(){
  uint8_t cpu_idle = (cpu_diff * 100) / (500 * 1000);  // usec;
  uint8_t cpu_usage = 100 - ((cpu_diff * 100) / (500 * 1000));  // usec;;
  theTerm.printf("Idle: %d%% Usage: %d%% \r\n", cpu_idle, cpu_usage);
}
void reportSystemStats(){
  mbed_stats_sys_get(&sys_stats);
  theTerm.printf("=============================== SYSTEM INFO  ================================\r\n");
  theTerm.printf("Mbed OS Version: %ld \r\n", sys_stats.os_version);
  theTerm.printf("CPU ID: 0x%lx \r\n", sys_stats.cpu_id);
  theTerm.printf("Compiler ID: %d \r\n", sys_stats.compiler_id);
  theTerm.printf("Compiler Version: %ld \r\n", sys_stats.compiler_version);

  for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
      if (sys_stats.ram_size[i] != 0) {
          theTerm.printf("RAM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.ram_start[i], sys_stats.ram_size[i]);
      }
  }
  for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
      if (sys_stats.rom_size[i] != 0) {
          theTerm.printf("ROM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.rom_start[i], sys_stats.rom_size[i]);
      }
  }
}

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
    thread_stats = new mbed_stats_thread_t[max_thread_count];
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
