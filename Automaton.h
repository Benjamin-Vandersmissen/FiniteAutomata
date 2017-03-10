//
// Created by Benjamin on 06/03/2017.
//

#ifndef PARSER_AUTOMATON_H
#define PARSER_AUTOMATON_H
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

class State;


class Transition{
private:
    State* begin;
    State* end;
    char input;
public:
    Transition(State *begin, State* end, char input);

    State * getBegin() const;

    State * getEnd() const;

    char getInput() const;

};

class State{
private:
    std::string name;
    bool accepting;
    bool starting;
    std::vector<Transition*> transitions;
public:
    State(const std::string &name, bool accepting, bool starting);

    bool isAccepting() const;

    bool isStarting() const;

    const std::string &getName() const;

    void addTransition(Transition* transition);

    bool hasEachTransitionOnce(std::vector<char> inputs);

    const std::vector<Transition *> &getTransitions() const;

    State* transition(char c);
};


std::ostream& operator<<(std::ostream& stream, Transition& transition);

class Automaton {
private:
    std::string type;
    char epsilon;
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<char> alphabet;
    State* startingState = NULL;
    std::vector<State*> acceptStates = {};
public:
    Automaton(const std::string &type, char epsilon, const std::vector<State *> &states,
              const std::vector<Transition *> &transitions, const std::vector<char> &alphabet);

    State* getState(std::string name);
    void toDotFormat(std::ostream& stream);
    State* transition(State* state, std::string string);
};


#endif //PARSER_AUTOMATON_H
