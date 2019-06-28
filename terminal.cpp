#include "terminal.hpp"
#include <string>
#include "mbed.h"
//Serial* serialTerminal::debug; // tx, rx
//Serial* serialTerminal::term;
bool serialTerminal::commandReady;
string serialTerminal::serialBuffer;
string serialTerminal::commandBuffer;
recallBuffer* serialTerminal::myBuffer;// recall last 10 commands
serialTerminal* serialTerminal::theTerm=nullptr;
RawSerial* serialTerminal::debug = new RawSerial(PC_10,PC_11,115200);
//void serialTerminal::serialIRQHandler();
string banner=
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\nhello!!!"
"\\nterm$ ";

char LEFTKEY   =0x44;
char RIGHTKEY  =0x43;
char UPKEY     =0x41;
char DOWNKEY   =0x42;
// char LEFTKEY   =0x4B;
// char RIGHTKEY  =0x4D;
// char UPKEY     =0x48;
// char DOWNKEY   =0x50;

char arrowSeq1=0x1b;
char arrowSeq2=0x5b;
// char arrowSeq1=0xc3;
// char arrowSeq2=0xa0;
void serialTerminal::printStr(string& inStr){
  theTerm->printf(inStr.c_str());
}
void serialTerminal::printDebug(string inStr){
  debug->printf("[DEBUG] ");
  debug->printf((char*)inStr.c_str());
}
void serialTerminal::printDebug(const char* inStr){
  debug->printf("[DEBUG] ");
  debug->printf(inStr);
}



recallBuffer::recallBuffer(uint8_t inMax):max(inMax){
}

void recallBuffer::add(std::string inStr){
    if (buffer.size()>=max){
        buffer.pop_back();
    }
    buffer.push_front(inStr);
    //return StatusResult{true;}
}
std::string recallBuffer::at(uint8_t pos){
    uint8_t limit;
    if (buffer.size()<max)
        limit=buffer.size();
    else
        limit=max;
    while(pos>=limit){
        pos-=limit;
    }
    return buffer.at(pos);
}
size_t recallBuffer::getSize(){
    return buffer.size();
}

serialTerminal::serialTerminal(PinName tx,PinName rx,int inbaud):RawSerial(tx,rx){// tx, rx
  //term=new Serial(tx,rx);
  myBuffer= new recallBuffer(10);
  theTerm=this;
  // theTerm->printf("before IRQ attach\n");
  theTerm->attach(serialIRQHandler);
  // theTerm->printf("after IRQ attach\n");
  theTerm->baud(inbaud);
  theTerm->printf(banner.c_str());

  theTerm->putc(0x1b);
  theTerm->printf("[4;h");
  // debug=this;

}

void serialTerminal::serialIRQHandler(){


    static bool arrowWatch5B;
    static bool arrowWatch;
    static uint8_t cursor;
    static uint8_t upkeys;
    while(theTerm->readable()){
        char theChar=(char) theTerm->getc();
        debug->printf("%02X\n", theChar);

        if(theChar=='\b'&&serialBuffer.size()>0)
        {
          //set to replacement mode
          theTerm->putc(0x1b);
          theTerm->printf("[4;l");

          theTerm->putc(theChar); //print back what you typed in
          theTerm->printf(" \b");
          serialBuffer.pop_back();//  pop last
          cursor--;
          // debug->printf("len: %u\n",length);
          // debug->printf("buff: ");
          // printDebug(serialBuffer);
          // debug->printf("\n");

          // set Insert mode
          theTerm->putc(0x1b);
          theTerm->printf("[4;h");
        }
        else if(theChar==arrowSeq1){ // got ESC char
          arrowWatch5B=true;
          //printDebug("seq1\n");
        }
        else if(theChar==arrowSeq2&&arrowWatch5B){ // ESC got pressed previously
          arrowWatch5B=false;
          arrowWatch=true;
          printDebug("seq2\n");
        }
        else if((theChar==LEFTKEY||theChar==RIGHTKEY||theChar==UPKEY||theChar==DOWNKEY)&&arrowWatch){
          printDebug("seq3\n");
          arrowWatch=false;

          if(theChar==LEFTKEY){
            printDebug("left key detected\n");
            if(cursor>0){
              cursor--;
              theTerm->putc(0x1b);
              theTerm->printf("[1;D");
            }
          }
          else if(theChar==RIGHTKEY){
            if(cursor<serialBuffer.length()){
              cursor++;
              theTerm->putc(0x1b);
              theTerm->printf("[1;C");
            }
            printDebug("right key detected\n");
          }
          else if(theChar==UPKEY){
            printDebug("up key detected\n");
            theTerm->putc(0x1b);
            theTerm->printf("[1;M"); // clear current row

            serialBuffer=myBuffer->at(upkeys);
            upkeys++;

            theTerm->putc(0x1b);
            theTerm->printf("[0;`"); // bounce cursor back to head

            theTerm->printf("term$ ");
            theTerm->printf(serialBuffer.c_str());
            cursor=serialBuffer.length();
          }
          else if(theChar==DOWNKEY){
            printDebug("down key detected\n");
            if(upkeys>0){


            upkeys--;
            serialBuffer=myBuffer->at(upkeys);
            theTerm->putc(0x1b);
            theTerm->printf("[1;M"); // clear current row
            theTerm->putc(0x1b);
            theTerm->printf("[0;`"); // bounce cursor back to head

            theTerm->printf("term$ ");
            theTerm->printf(serialBuffer.c_str());
            cursor=serialBuffer.length();
            }

          }
        }

        else if(theChar!='\b'&&theChar!='\t'&&theChar!='\r'&&theChar!='\n'){// do not append control characters
          // arrowWatch sequece failed
          arrowWatch=false;
          arrowWatch5B=false;
          theTerm->putc(theChar); //print back what you typed in
          serialBuffer.insert(cursor,1,theChar); // append latest char to string

          cursor++;

          debug->printf("len: %u\n",serialBuffer.length());
          debug->printf("buff: ");
          printDebug(serialBuffer);
          debug->printf("\n");

        }
        if(theChar=='\n'){
          if(serialBuffer.size()>1){
            commandBuffer=toUpper(serialBuffer);
            commandReady=true;
            myBuffer->add(serialBuffer);
            serialBuffer="";
            cursor=0;
            upkeys=0;
          }
          else{
            theTerm->printf("\nterm$ ");
            serialBuffer="";
            cursor=0;
          }
        }

    }


}
