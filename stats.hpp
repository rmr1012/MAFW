#include "mbed.h"


mbed_stats_heap_t   heap_stats;
mbed_stats_cpu_t    cpu_stats;
mbed_stats_sys_t    sys_stats;

mbed_stats_thread_t *thread_stats;
uint8_t   thread_count;
uint8_t   max_thread_count=10;

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
  printf("Idle: %d%% Usage: %d%% \r\n", cpu_idle, cpu_usage);
}
void reportSystemStats(){
  mbed_stats_sys_get(&sys_stats);
  printf("=============================== SYSTEM INFO  ================================\r\n");
  printf("Mbed OS Version: %ld \r\n", sys_stats.os_version);
  printf("CPU ID: 0x%lx \r\n", sys_stats.cpu_id);
  printf("Compiler ID: %d \r\n", sys_stats.compiler_id);
  printf("Compiler Version: %ld \r\n", sys_stats.compiler_version);

  for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
      if (sys_stats.ram_size[i] != 0) {
          printf("RAM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.ram_start[i], sys_stats.ram_size[i]);
      }
  }
  for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
      if (sys_stats.rom_size[i] != 0) {
          printf("ROM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.rom_start[i], sys_stats.rom_size[i]);
      }
  }
}
void initStats(){
  thread_stats = new mbed_stats_thread_t[max_thread_count];
  statsTicker.attach(updateCPUStats,0.5);
}
