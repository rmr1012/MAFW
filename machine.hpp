//
//  machine.hpp
//  FSM_test
//
//  Created by Dennis Ren on 6/23/19.
//  Copyright © 2019 30. All rights reserved.
//

#ifndef machine_hpp
#define machine_hpp

#include <stdio.h>

#include <vector>

class State;
class Machine{
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

class State {
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
        IDLE(){printf( "   IDLE-ctor\n");};
        ~IDLE(){printf( "   IDLE-dtor\n");};
        void charging(Machine *m);
};
class CHARGING: public State
{
    public:
        CHARGING(){printf( "   CHARGING-ctor\n");};
        ~CHARGING(){printf( "   CHARGING-dtor\n");};
        void standby(Machine *m);
};
class STANDBY: public State
{
    public:
        STANDBY(){printf( "   STANDBY-ctor\n");};
        ~STANDBY(){printf( "   STANDBY-dtor\n");};
        void loaded(Machine *m);
};
class LOADED: public State
{
    public:
        LOADED(){printf( "   LOADED-ctor\n");
        };~LOADED(){printf( "   LOADED-dtor\n");};
        void discharge(Machine *m);
};

class DISCHARGE: public State
{
public:
    DISCHARGE(){printf( "   DISCHARGE-ctor\n");};
    ~DISCHARGE(){printf( "   DISCHARGE-dtor\n");};
    void idle(Machine *m);
};

#endif /* machine_hpp */
