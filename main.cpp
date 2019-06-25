//
// #include "MAFW.h"
// // #include "stats_report.h"
// #include "machine.hpp"
// #include "rtos/Mutex.h"
// #include "mbed.h"
// #include "stage.hpp"
// Serial debugUART(TERM_TX,TERM_RX);
// DigitalOut led1(LED2);
// // AnalogIn   ch0(PA_0);
// // AnalogIn   ch1(PA_1);
// // AnalogIn   ch2(PA_2);
// // AnalogIn   ch3(PA_3);
// //
// InterruptIn armSwitch(PB_10);
// InterruptIn loadMeter(PC_15);
//
// Timer t;
// #define SLEEP_TIME                  100// (usec)
// #define PRINT_AFTER_N_LOOPS         20
//
//
// // main() runs in its own thread in the OS
// int tik,tok;
// // void riseISR(){
// //   tik=t.read_us();
// //   printf("rise \n");
// //
// // }
// // void fallISR(){
// //   tok=t.read_us();
// //   printf("fall + %dus\n",tok-tik);
// // }
// EventQueue queue;
//
// void uartISRNonMutex() {
//     // NVIC_DisableIRQ(USART1_IRQn);
//     //printf("%02X\n", debugUART.getc());
//     lock();
//     wait_ms(100);
//     unlock();
//     // NVIC_EnableIRQ(USART1_IRQn);
// }
//
//
// int main()
// {
//     debugUART.baud(115200);
//     printf("helloworld");
//     Thread eventThread;
//     eventThread.start(callback(&queue, &EventQueue::dispatch_forever));
//     InterruptIn sw(PC_13);
//     sw.rise(queue.event(&uartISRNonMutex));
//
//     t.start();
//     // SystemReport sys_state( SLEEP_TIME * PRINT_AFTER_N_LOOPS /* Loop delay time in ms */);
//     int count=0;
//     wait_ms(500);
//     // Meter meter0(0, PC_14, PC_13);
//     // Stage stage0(0 , &meter0, 0.1 , PA_0 , PA_1 , PC_10 );
//     // stage0.armStage();
//     Machine MA();
//
//     // meter0.assignTrigger(&stage0.discharge);
//     while (true) {
//         // Blink LED and wait 0.5 seconds
//         led1 = !led1;
//         // printf("rise \n");
//         // debugUART.printf("pa\n");
//         // debugUART.printf("val: %3.3f\t%3.3f\t%3.3f\t%3.3f\t\n",ch0.read(),ch1.read(),ch2.read(),ch3.read());
//         wait_ms(SLEEP_TIME);
//
//         // if ((0 == count) || (PRINT_AFTER_N_LOOPS == count)) {
//         //     // Following the main thread wait, report on the current system status
//         //     sys_state.report_state();
//         //     count = 0;
//         // }
//         //  ++count;
//     }
// }
#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Serial pc(USBTX, USBRX);

void callback() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    printf("%c\n", pc.getc());
    led2 = !led2;
}

int main() {
    // pc.baud(115200);
    pc.attach(&callback);

    while (1) {
        led1 = !led1;
        wait(0.25);
    }
}
