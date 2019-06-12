#include <utility>
#include <string>
#include <map>
#include <regex>
#include "mbed.h"

#define BAUDRATE 115200
#define WDT_LENGTH 15000
#define LED1 LED1
#define TERM_TX USBTX
#define TERM_RX USBRX
#define DEBUG_TX PTC4
#define DEBUG_RX PTC3
#define MYI2C_SDA PTD9
#define MYI2C_SCL PTD8
#define CLKMUL 1
#define MYSPI_SCK PTD5
#define MYSPI_MISO PTD7
#define MYSPI_MOSI PTD6

Serial debugUART(TERM_TX,TERM_RX);
