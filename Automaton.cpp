//
// Created by Benjamin on 06/03/2017.
//

#include <stdexcept>
#include <iostream>
#include "Automaton.h"

State::State(const std::string &name, bool accepting, bool starting) : name(name), accepting(accepting),
                                                                       starting(starting) {}

bool State::isAccepting() const {
    return accepting;
}

bool State::isStarting() const {
    return starting;
}

const std::string &State::getName() const {
    return name;
}

void State::addTransition(Transition *transition) {
    if (transition->getBegin() == this->name){
        this->transitions.push_back(transition);
    }
}

bool State::hasEachTransitionOnce(std::vector<char> inputs) {
    for(char c : inputs){
        bool succes = false;
        for(Transition* transition: this->transitions){
            if (c == transition->getInput()){
                if (!succes){
                    succes = true;
                }else{
                    return false; //input occurs double
                }
            }
        }
        if (!succes){
            return false; //misses an input
        }
    }
    return true;
}

const std::vector<Transition *> &State::getTransitions() const {
    return transitions;
}

Transition::Transition(const std::string &begin, const std::string &end, char input) : begin(begin), end(end),
                                                                                       input(input) {}

const std::string &Transition::getBegin() const {
    return begin;
}

const std::string &Transition::getEnd() const {
    return end;
}

char Transition::getInput() const {
    return input;
}

std::ostream& operator<<(std::ostream& stream, Transition& transition){
    stream <<  transition.getBegin() << ": " <<  transition.getInput() << " -> " << transition.getEnd();
    return stream;
}


Automaton::Automaton(const std::string &type, char epsilon, const std::vector<State *> &states,
                     const std::vector<Transition *> &transitions, const std::vector<char> &alphabet) : type(type),
                                                                                                        epsilon(epsilon),
                                                                                                        states(states),
                                                                                                        transitions(
                                                                                                                transitions),
                                                                                                        alphabet(
                                                                                                                alphabet) {
    for(State* state1 : this->states){
        for(Transition* transition : this->transitions){
            state1->addTransition(transition);
        }
    }

    bool test = false;
    for(State* state: this->states){
        if(state->isStarting()){
            test = true;
            if (this->startingState == NULL){
                this->startingState = state;
            }else{
                throw(std::invalid_argument("The automaton can have only one starting state"));
            }
        }
    }

    if(!test){
        throw(std::invalid_argument("The automaton needs a starting state"));
    }

    for(State* state: this->states){
        if (state->isAccepting()){
            this->acceptStates.push_back(state);
        }
    }
    if(this->acceptStates.size() == 0){
        throw(std::invalid_argument("The automaton needs at least one accepting state"));
    }

    for(State* state: this->states){
        for(Transition* transition: state->getTransitions()){
            std::cerr << *transition << std::endl;
        }
        std::cerr << "~~~~~~" << std::endl;
    }
    if(type == "DFA"){
        for(State* state: this->states){
            if(!state->hasEachTransitionOnce(this->alphabet)){
                throw(std::invalid_argument("Each state of a DFA needs to have a transition for each character in the alphabet and only once"));
            }
        }
    }
    if (type == "eNFA"){
        if (std::find(alphabet.begin(), alphabet.end(),epsilon) != alphabet.end()){
            throw(std::invalid_argument("epsilon can't be a part of the alphabet"));
        }
    }
}

State *Automaton::getState(std::string name) {
    for(State* state : this->states){
        if (state->getName() == name){
            return state;
        }
    }
    return NULL;
}

void Automaton::toDotFormat(std::ostream &stream) {
    stream << "digraph " << this->type << "{" << std::endl;
    stream << "\tDummy ->" << this->startingState->getName() << ";" << std::endl;
    stream << "\tDummy [style=invis];" << std::endl;
    for(State* endstate : this->acceptStates){
        stream << "\t" << endstate->getName() << "[shape=doublecircle];" << std::endl;
    }
    for(Transition* transition : this->transitions){
        stream << "\t" <<transition->getBegin() << " -> " << transition->getEnd() << " [label=" << transition->getInput() <<"];" << std::endl;
    }
    stream << "}" << std::endl;
}
