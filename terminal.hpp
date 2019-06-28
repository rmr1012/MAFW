#ifndef IHMDIAGSTERMINAL_H
#define IHMDIAGSTERMINAL_H

#include <string>
#include <deque>
#include "mbed.h"
#include "utilities.hpp"




class recallBuffer{
  public:
    recallBuffer(uint8_t);
    uint8_t max=20;
    std::deque<std::string> buffer;
    void add(std::string inStr);
    //void clear();
    std::string at(uint8_t pos); // check if 3 chars are legal
    size_t getSize();
};


class serialTerminal: public RawSerial{
public:
  serialTerminal(PinName,PinName,int);// tx, rx
  
  static void serialIRQHandler();
  static serialTerminal* theTerm;
  static bool commandReady;

  static string serialBuffer;
  static string commandBuffer;
  static recallBuffer* myBuffer;// recall last 10 commands
  static RawSerial* debug;
  static void printDebug(string);
  static void printDebug(const char*);
  static void printStr(string&);
};



#endif
