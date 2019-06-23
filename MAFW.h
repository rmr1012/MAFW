#include <utility>
#include <string>
#include <map>
#include <regex>
#include "mbed.h"
#include "stage.hpp"

#define BAUDRATE 115200
#define WDT_LENGTH 15000
#define TERM_TX USBTX
#define TERM_RX USBRX
#define DEBUG_TX PTC4
#define DEBUG_RX PTC3


Serial debugUART(TERM_TX,TERM_RX);
