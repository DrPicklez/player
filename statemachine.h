#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <json/json.h>
#include <fstream>
#include <iostream>
//#define JSON_IS_AMALGAMATION


class StateMachine
{
public:

    class State{
        public:
            std::string name;
            uint32_t startTime;
            uint32_t endTime;
            int8_t position;
            uint32_t length;
            bool isTransition;
            int8_t targetPosition;
    };

    StateMachine();
    bool parseFile();
    bool updateTime(uint32_t _time);
    void setTargetPosition(int position);
    State getNextState();

    int currentPosition;
    int targetPosition;
    State currentState;
    uint32_t currentTime;


    std::vector<State> readFile();
    std::vector<State> states;
    std::vector<State> transitionStates;
};

#endif // STATEMACHINE_H
