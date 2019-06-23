
#include "MAFW.h"
#include "stats_report.h"

DigitalOut led1(LED2);
// AnalogIn   ch0(PA_0);
// AnalogIn   ch1(PA_1);
// AnalogIn   ch2(PA_2);
// AnalogIn   ch3(PA_3);
//
// InterruptIn S1P1(PC_13);
Timer t;

#define SLEEP_TIME                  100// (usec)
#define PRINT_AFTER_N_LOOPS         20

// main() runs in its own thread in the OS
int tik,tok;
// void riseISR(){
//   tik=t.read_us();
//   printf("rise \n");
//
// }
// void fallISR(){
//   tok=t.read_us();
//   printf("fall + %dus\n",tok-tik);
// }


int main()
{
    debugUART.baud(115200);
    t.start();
    SystemReport sys_state( SLEEP_TIME * PRINT_AFTER_N_LOOPS /* Loop delay time in ms */);
    int count=0;
    wait_ms(500);
    Meter meter0(0, PC_14, PC_15);
    Stage stage0(0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 , PC_13);
    // meter0.assignTrigger(&stage0.discharge);
    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        // printf("rise \n");
        // debugUART.printf("pa\n");
        // debugUART.printf("val: %3.3f\t%3.3f\t%3.3f\t%3.3f\t\n",ch0.read(),ch1.read(),ch2.read(),ch3.read());
        wait_ms(SLEEP_TIME);

        // if ((0 == count) || (PRINT_AFTER_N_LOOPS == count)) {
        //     // Following the main thread wait, report on the current system status
        //     sys_state.report_state();
        //     count = 0;
        // }
        //  ++count;
    }
}
