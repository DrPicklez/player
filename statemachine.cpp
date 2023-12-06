#include "statemachine.h"
//using namespace std;
//#define JSON_IS_AMALGAMATION

StateMachine::StateMachine(){
    parseFile();
    currentState = states[0];
    targetPosition = 1;

//    for(auto state: transitionStates){
//        std::cout << state.name << std::endl;
//    }

}

bool StateMachine::parseFile(){
    std::ifstream animations_file("/home/william/Projects/videoPlayer/Player/Player/AnimationClips.json", std::ifstream::binary);
    Json::Value animations;
    Json::Reader reader;
    if(! reader.parse(animations_file, animations, false)){
        std::cout << "json no good" << std::endl;
        return false;
    }
    std::vector<StateMachine::State> _states;
    for (Json::Value::ArrayIndex i = 0; i != animations.size(); i++){
        State state;
        state.name = animations[i].get("Name", "nan" ).asString();
        state.startTime = animations[i].get("startTime", "nan" ).asInt();
        state.endTime = animations[i].get("endTime", "nan" ).asInt();
        state.length = animations[i].get("length", "nan" ).asInt();
        state.isTransition = animations[i].get("isTransition", "nan" ).asBool();
        state.position = animations[i].get("position", "nan" ).asInt();
        state.targetPosition = animations[i].get("targetPosition", "nan" ).asInt();
        if(state.isTransition){
            transitionStates.push_back(state);
        }
        else {
            states.push_back(state);
        }
    }
    return true;
}


StateMachine::State StateMachine::getNextState(){
    State _nextState;

    for(auto state : transitionStates){
        if(state.position == currentPosition){
            if(state.targetPosition == targetPosition){
                return state;
            }
            else {
                int aimFor = currentPosition;
                if(currentState.position > targetPosition){
                    aimFor --;
                }
                else if(currentState.position < targetPosition){
                    aimFor ++;
                }
                if(state.targetPosition == aimFor){
                    return state;
                }
            }
        }
    }

    return _nextState;
}


void StateMachine::setTargetPosition(int position){
    targetPosition = position;
}

bool StateMachine::updateTime(uint32_t _time){
    bool isNext = false;
    currentTime = _time;
    std::cout << currentTime << std::endl;
    if(currentTime >= currentState.endTime){
        currentState = getNextState();
        isNext = true;
        std::cout << "Change" << currentState.name << std::endl;
    }
    return isNext;
}

/*
std::vector<StateMachine::State> StateMachine::readFile(){
    std::ifstream animations_file("/home/william/Projects/videoPlayer/Player/Player/AnimationClips.json", std::ifstream::binary);
    Json::Value animations;
    Json::Reader reader;
    if(! reader.parse(animations_file, animations, false)){
        std::cout << "json noGood" << std::endl;
    }
    std::vector<StateMachine::State> states;
    for (Json::Value::ArrayIndex i = 0; i != animations.size(); i++){
        State _state;
        state.name = animations[i].get("Name", "nan" ).asString();
        state.startTime = animations[i].get("startTime", "nan" ).asInt();
        state.endTime = animations[i].get("endTime", "nan" ).asInt();
        state.length = animations[i].get("length", "nan" ).asInt();
        state.isTransition = animations[i].get("isTransition", "nan" ).asBool();
        state.position = animations[i].get("position", "nan" ).asInt();
        _states.push_back(state);
    }
    return _states;
}
*/


