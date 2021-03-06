/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef STATS_REPORT_H
#define STATS_REPORT
#include "mbed.h"
#include "utilities.hpp"


/**
 *  System Reporting library. Provides runtime information on device:
 *      - CPU sleep, idle, and wake times
 *      - Heap and stack usage
 *      - Thread information
 *      - Static system information
 */
class SystemReport: public Inarticulate{
    mbed_stats_cpu_t    cpu_stats;
    mbed_stats_sys_t    sys_stats;

    mbed_stats_thread_t *thread_stats;
    uint8_t   thread_count;
    uint8_t   max_thread_count;
    uint32_t  sample_time_ms;

public:
    /**
     *  SystemReport - Sample rate in ms is required to handle the CPU percent awake logic
     */
    SystemReport(uint32_t sample_rate) : max_thread_count(1), sample_time_ms(sample_rate)
    {
        // thread_stats = new mbed_stats_thread_t[max_thread_count];

        // Collect the static system information
        mbed_stats_sys_get(&sys_stats);

        bprintf("=============================== SYSTEM INFO  ================================\r\n");
        bprintf("Mbed OS Version: %ld \r\n", sys_stats.os_version);
        bprintf("CPU ID: 0x%lx \r\n", sys_stats.cpu_id);
        bprintf("Compiler ID: %d \r\n", sys_stats.compiler_id);
        bprintf("Compiler Version: %ld \r\n", sys_stats.compiler_version);

        for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
            if (sys_stats.ram_size[i] != 0) {
                bprintf("RAM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.ram_start[i], sys_stats.ram_size[i]);
            }
        }
        for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
            if (sys_stats.rom_size[i] != 0) {
                bprintf("ROM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.rom_start[i], sys_stats.rom_size[i]);
            }
        }
    }

    ~SystemReport(void)
    {
        // free(thread_stats);
    }

    /**
     *  Report on each Mbed OS Platform stats API
     */
    void report_state(void)
    {
        report_cpu_stats();
        report_thread_stats();

        // Clear next line to separate subsequent report logs
        bprintf("\r\n");
    }

    /**
     *  Report CPU idle and awake time in terms of percentage
     */
    void report_cpu_stats(void)
    {
        static uint64_t prev_idle_time = 0;

        bprintf("================= CPU STATS =================\r\n");

        // Collect and print cpu stats
        mbed_stats_cpu_get(&cpu_stats);

        uint64_t diff = (cpu_stats.idle_time - prev_idle_time);
        uint8_t idle = (diff * 100) / (sample_time_ms * 1000);  // usec;
        uint8_t usage = 100 - ((diff * 100) / (sample_time_ms * 1000));  // usec;;
        prev_idle_time = cpu_stats.idle_time;

        bprintf("Idle: %d%% Usage: %d%% \r\n", idle, usage);
    }

    /**
     *  Report current heap stats. Current heap refers to the current amount of
     *  allocated heap. Max heap refers to the highest amount of heap allocated
     *  since reset.
     */


    /**
     *  Report active thread stats
     */
    void report_thread_stats(void)
    {
        bprintf("================ THREAD STATS ===============\r\n");

        // Collect and print running thread stats
        int count = mbed_stats_thread_get_each(thread_stats, max_thread_count);

        for (int i = 0; i < count; i++) {
            bprintf("ID: 0x%lx \r\n",        thread_stats[i].id);
            bprintf("Name: %s \r\n",         thread_stats[i].name);
            bprintf("State: %ld \r\n",       thread_stats[i].state);
            bprintf("Priority: %ld \r\n",    thread_stats[i].priority);
            bprintf("Stack Size: %ld \r\n",  thread_stats[i].stack_size);
            bprintf("Stack Space: %ld \r\n", thread_stats[i].stack_space);
        }
    }
};

#endif // STATS_REPORT_H
