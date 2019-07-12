//
//  machine.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//
#ifndef MACHINE_H
#define MACHINE_H


#include "mbed.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <utility>
#include "stage.hpp"
#include "utilities.hpp"


// class Inarticulate{
// public:
//
//     // static void printf(const char * inStr){
//     //     buffer+=inStr;
//     // }
//     static void printf(const char *format, ...){
//         va_list args;
//         va_start(args, format);
//         char buff[100];
//         vsnprintf(buff, sizeof buff, format, args);
//         // std::string buffAsStdStr = buff;
//         va_end(args);
//         buffer+=buff;
//     }
//
//     static std::string getBuffer(){
//         std::string outbuf=buffer;
//         buffer="";
//         return outbuf;
//     }
//     static bool isReady(){
//         return buffer != "";
//     }
// private:
//     static std::string buffer;
// };

class State;
class Machine: public Inarticulate{
    public:
        Machine();
        // InterruptIn* armSwitch;
        // InterruptIn* loadMeter;
        State *current;
        void setCurrent(State *s);
        void idle();
        void charging();
        void standby();
        void loaded();
        void discharge();
        void appendStage(Stage* );
        void appendMeter(Meter* );
        string report();
      private:
        std::vector<Stage*> stages;
        std::vector<Meter*> meters;
};

class State : public Inarticulate{
    public:
        virtual void idle(Machine *m);
        virtual void charging(Machine *m);
        virtual void standby(Machine *m);
        virtual void loaded(Machine *m);
        virtual void discharge(Machine *m);
        string name;
        virtual string report();
};

class IDLE: public State
{
    public:
        IDLE(){printf( "   IDLE-ctor\n");};
        ~IDLE(){printf( "   IDLE-dtor\n");};
        void charging(Machine *m);
        string report();
        string name="idle";
};
class CHARGING: public State
{
    public:
        CHARGING(){printf( "   CHARGING-ctor\n");};
        ~CHARGING(){printf( "   CHARGING-dtor\n");};
        void standby(Machine *m);
        string report();
        string name="charging";
};
class STANDBY: public State
{
    public:
        STANDBY(){printf( "   STANDBY-ctor\n");};
        ~STANDBY(){printf( "   STANDBY-dtor\n");};
        void loaded(Machine *m);
        string report();
        string name="standby";
};
class LOADED: public State
{
    public:
        LOADED(){printf( "   LOADED-ctor\n");
        };~LOADED(){printf( "   LOADED-dtor\n");};
        void discharge(Machine *m);
        string report();
        string name="loaded";
};

class DISCHARGE: public State
{
public:
    DISCHARGE(){printf( "   DISCHARGE-ctor\n");};
    ~DISCHARGE(){printf( "   DISCHARGE-dtor\n");};
    void idle(Machine *m);
    string report();
    string name="discharge";
};

#endif /* machine_hpp */
