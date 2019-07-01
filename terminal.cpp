#include "terminal.hpp"
#include <string>
#include "mbed.h"
//Serial* serialTerminal::debug; // tx, rx
//Serial* serialTerminal::term;
bool serialTerminal::commandReady;
bool serialTerminal::ctrlc=false;
string serialTerminal::serialBuffer;
string serialTerminal::commandBuffer;
recallBuffer* serialTerminal::myBuffer;// recall last 10 commands
serialTerminal* serialTerminal::theTerm=nullptr;
RawSerial* serialTerminal::debug = new RawSerial(PB_6,PB_7,115200);

// Callback<void()> serialTerminal::commandCallback;
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
// void serialTerminal::attach(Callback<void()> inFunc){
//   commandCallback=inFunc;
// }


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


  theTerm->printf("\e[4;h");
  // debug=this;

}

void serialTerminal::serialIRQHandler(){


    static bool arrowWatcher5B;
    static bool arrowWatcher;
    static uint8_t cursor; //horizontal position
    static uint8_t upkeys;
    while(theTerm->readable()){
        char theChar=(char) theTerm->getc();
        debug->printf("%02X\n", theChar);

        if(theChar=='\b'&& serialBuffer.size()>0 && cursor>0) //BACKSPACE
        {
          theTerm->putc(theChar); //print back what you typed in
          theTerm->printf("\e[s"); // save cursor position

          cursor--;
          serialBuffer.erase(cursor,1);

          theTerm->printf("\rterm$ ");
          theTerm->printf("\e[K");
          theTerm->printf(serialBuffer.c_str());
          theTerm->printf(" ");

          theTerm->printf("\e[u");// restore cursor position

        }
        else if(theChar==0x03){//ctrl+c pressed
          theTerm->printf("\nterm$ ");
          ctrlc=true;
        }
        else if(theChar==arrowSeq1){ // got ESC char
          arrowWatcher5B=true;
          //printDebug("seq1\n");
        }
        else if(theChar==arrowSeq2&&arrowWatcher5B){ // ESC got pressed previously
          arrowWatcher5B=false;
          arrowWatcher=true;
          printDebug("seq2\n");
        }
        else if((theChar==LEFTKEY||theChar==RIGHTKEY||theChar==UPKEY||theChar==DOWNKEY)&&arrowWatcher){
          printDebug("seq3\n");
          arrowWatcher=false;

          if(theChar==LEFTKEY){
            printDebug("left key detected\n");
            if(cursor>0){
              cursor--;

              theTerm->printf("\e[1;D");
            }
          }
          else if(theChar==RIGHTKEY){
            if(cursor<serialBuffer.length()){
              cursor++;

              theTerm->printf("\e[1;C");
            }
            printDebug("right key detected\n");
          }
          else if(theChar==UPKEY){
            printDebug("up key detected\n");

            theTerm->printf("\e[1;M\r"); // clear current row

            serialBuffer=myBuffer->at(upkeys);
            upkeys++;


            theTerm->printf("\e[0;`"); // bounce cursor back to head

            theTerm->printf("term$ ");
            theTerm->printf(serialBuffer.c_str());
            cursor=serialBuffer.length();
          }
          else if(theChar==DOWNKEY){
            printDebug("down key detected\n");
            if(upkeys>=0){


            upkeys--;
            serialBuffer=myBuffer->at(upkeys);

            theTerm->printf("\e[1;M\r"); // clear current row // bounce cursor back to head

            // theTerm->printf("\e[0;`"); // bounce cursor back to head

            theTerm->printf("term$ ");
            theTerm->printf(serialBuffer.c_str());
            cursor=serialBuffer.length();
            }

          }
        }

        else if(theChar>=32){//theChar!='\b'&&theChar!='\t'&&theChar!='\r'&&theChar!='\n'){// do not append control characters
          // arrowWatcher sequece failed
          arrowWatcher=false;
          arrowWatcher5B=false;
          theTerm->putc(theChar); //print back what you typed in
          serialBuffer.insert(cursor,1,theChar); // append latest char to string

          cursor++;

          debug->printf("len: %u\n",serialBuffer.length());
          debug->printf("buff: ");
          printDebug(serialBuffer);
          debug->printf("\n");

        }
        if(theChar=='\n' | theChar=='\r'){
          if(serialBuffer.size()>1){
            commandBuffer=toUpper(serialBuffer);
            commandReady=true;
            // commandCallback();
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
