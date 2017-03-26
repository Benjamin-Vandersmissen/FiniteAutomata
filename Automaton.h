//
// Created by Benjamin on 06/03/2017.
//

#ifndef PARSER_AUTOMATON_H
#define PARSER_AUTOMATON_H
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>

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

    void setEnd(State *end);
};

class State{
private:
    std::string name;
    bool accepting;
    bool starting;
    std::vector<Transition*> transitions;
public:
    std::vector<State*> Eclose(char epsilon, std::vector<State*>& states );

    State(const std::string &name, bool accepting, bool starting);

    bool isAccepting() const;

    bool isStarting() const;

    const std::string &getName() const;

    void addTransition(Transition* transition);

    bool hasEachTransitionOnce(std::vector<char> inputs);

    const std::vector<Transition *> &getTransitions() const;

    State* transition(char c);

    std::vector<State*> transitionNFA(char c);

    State(const std::string &name);

    void reacheableStates(std::vector<State*>& states);

    void setName(const std::string &name);

    void setAccepting(bool accepting);

    void setStarting(bool starting);
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
protected:
    bool TableFillingStep(State *state1, State *state2, std::vector<std::vector<bool>> &table);
public:
    Automaton(const std::string &type, char epsilon, const std::vector<State *> &states,
              const std::vector<Transition *> &transitions, const std::vector<char> &alphabet);

    State* getState(std::string name);


    State *getStartingState() const;

    const std::string &getType() const;

    const std::vector<State *> &getStates() const;

    const std::vector<Transition *> &getTransitions() const;

    const std::vector<char> &getAlphabet() const;

    const std::vector<State *> &getAcceptStates() const;

    void toDotFormat(std::ostream& stream);

    void toJSon(std::ostream& stream);

    State* transition(State* state, std::string string);

    std::vector<State*> transitionNFA(State* state, std::string string);

    std::vector<State*> reachableStates();

    void deleteState(State* state);

    void deleteAllUnreachableStates();

    std::vector<std::vector<State *>> TableFilling(bool compare = false, std::ostream &stream = std::cout);

    std::vector<State*> Eclose(State* state);

    void toDFA();
};

bool areEquivalent(Automaton *automaton1, Automaton *automaton2);
Automaton* getNFAFromRegex(std::string regex);

#endif //PARSER_AUTOMATON_H
