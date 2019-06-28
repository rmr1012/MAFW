//
//  machine.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//

#ifndef machine_hpp
#define machine_hpp
#include "mbed.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <utility>

class Inarticulate{
public:

    static void bprintf(const char * inStr){
        buffer+=inStr;
    }
    static string getBuffer(){
        string outbuf=buffer;
        buffer="";
        return outbuf;
    }
    static bool isReady(){
        return buffer != "";
    }
private:
    static string buffer;
};

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
};

class State : public Inarticulate{
    public:
        virtual void idle(Machine *m);
        virtual void charging(Machine *m);
        virtual void standby(Machine *m);
        virtual void loaded(Machine *m);
        virtual void discharge(Machine *m);
};

class IDLE: public State
{
    public:
        IDLE(){bprintf( "   IDLE-ctor\n");};
        ~IDLE(){bprintf( "   IDLE-dtor\n");};
        void charging(Machine *m);
};
class CHARGING: public State
{
    public:
        CHARGING(){bprintf( "   CHARGING-ctor\n");};
        ~CHARGING(){bprintf( "   CHARGING-dtor\n");};
        void standby(Machine *m);
};
class STANDBY: public State
{
    public:
        STANDBY(){bprintf( "   STANDBY-ctor\n");};
        ~STANDBY(){bprintf( "   STANDBY-dtor\n");};
        void loaded(Machine *m);
};
class LOADED: public State
{
    public:
        LOADED(){bprintf( "   LOADED-ctor\n");
        };~LOADED(){bprintf( "   LOADED-dtor\n");};
        void discharge(Machine *m);
};

class DISCHARGE: public State
{
public:
    DISCHARGE(){bprintf( "   DISCHARGE-ctor\n");};
    ~DISCHARGE(){bprintf( "   DISCHARGE-dtor\n");};
    void idle(Machine *m);
};

#endif /* machine_hpp */
