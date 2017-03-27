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
    if (transition->getBegin()->getName() == this->name && std::find(this->transitions.begin(), this->transitions.end(), transition) == this->transitions.end()){
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

State *State::transition(char c) {
    for(Transition* transition : this->transitions){
        if (c == transition->getInput()){
            return transition->getEnd();
        }
    }
}

State::State(const std::string &name) : name(name) {
}

void State::reacheableStates(std::vector<State *> &states) {
    for(Transition* transition: this->transitions){
        if (std::find(states.begin(), states.end(), transition->getEnd()) == states.end()){
            states.push_back(transition->getEnd());
            transition->getEnd()->reacheableStates(states);
        }
    }
}

void State::setName(const std::string &name) {
    State::name = name;
}

std::vector<State *> State::Eclose(char epsilon, std::vector<State *> &states) {
    //should only be called from the parent automaton, to determine the epsilon and to check if the automaton really is an eNFA
    for(Transition* transition : this->transitions){
        if (transition->getInput() == epsilon && std::find(states.begin(), states.end(), transition->getEnd()) == states.end()){
            states.push_back(transition->getEnd());
            transition->getEnd()->Eclose(epsilon, states);
        }
    }
    return states;
}

void State::setAccepting(bool accepting) {
    State::accepting = accepting;
}

void State::setStarting(bool starting) {
    State::starting = starting;
}

std::vector<State *> State::transitionNFA(char c) {
    std::vector<State*> states = {};
    for(Transition* transition: this->transitions){
        if (transition->getInput() == c){
            states.push_back(transition->getEnd());
        }
    }
    return states;
}

Transition::Transition(State* begin, State* end, char input) : begin(begin), end(end),
                                                                                       input(input) {}

State * Transition::getBegin() const {
    return begin;
}

State * Transition::getEnd() const {
    return end;
}

char Transition::getInput() const {
    return input;
}

void Transition::setEnd(State *end) {
    Transition::end = end;
}


std::ostream& operator<<(std::ostream& stream, Transition& transition){
    stream <<  transition.getBegin()->getName() << ": " <<  transition.getInput() << " -> " << transition.getEnd()->getName();
    return stream;
}

bool areEquivalent(Automaton *automaton1, Automaton *automaton2) {
    if(automaton1->getType() != "DFA" || automaton2->getType() != "DFA"){
        throw(std::invalid_argument("Both automata need to be DFAs"));
    }
    std::vector<State*> allStates = automaton1->getStates();
    allStates.insert(allStates.end(), automaton2->getStates().begin(), automaton2->getStates().end());
    std::vector<State*> acceptStates = automaton1->getAcceptStates();
    acceptStates.insert(acceptStates.end(), automaton2->getAcceptStates().begin(), automaton2->getAcceptStates().end());
    std::vector<char> alphabet = automaton1->getAlphabet();
    alphabet.insert(alphabet.end(), automaton2->getAlphabet().begin(), automaton2->getAlphabet().end());
    std::vector<Transition*> transitions = automaton1->getTransitions();
    transitions.insert(transitions.end(), automaton2->getTransitions().begin(), automaton2->getTransitions().end());

    automaton2->getStartingState()->setStarting(false);

    Automaton * totalAutomaton = new Automaton("NFA", ' ',allStates, transitions, alphabet);
    automaton2->getStartingState()->setStarting(true);
    std::ofstream stream("../outputTable.txt");
    std::vector<std::vector<State*>> states = totalAutomaton->TableFilling(true, stream);
    std::cerr << states.size() << std::endl;
    for(std::vector<State*> temp: states){
        if (std::find(temp.begin(), temp.end(), automaton1->getStartingState()) != temp.end() && std::find(temp.begin(), temp.end(), automaton2->getStartingState()) != temp.end()){
            return true;
        }
    }
    return false;
}

Automaton *getNFAFromRegex(std::string regex) {
    typedef struct{
        std::vector<State*> states;
        std::vector<Transition*> transtions;
        State* start;
        State* end;
    }tempAutomaton;

    std::vector<char> alphabet = {};
    int statenumber = 1;
    State* state = new State("Q0", true, true);
    State* beginstate = state;
    State* endstate = state;
    State* previousstate = state;
    char epsilon = '\u03B5';
    std::vector<State*> states = {state};
    std::vector<Transition*> transitions = {};
    std::vector<State*> brackets = {};
    State* temp = NULL;
    for(char c : regex){
        std::string specialChars = "()*+. ";
        if (specialChars.find(c) == std::string::npos) {
            if (std::find(alphabet.begin(), alphabet.end(), c) == alphabet.end()) {
                alphabet.push_back(c);
            }
            previousstate = states.back();

            endstate->setAccepting(false);
            states.push_back(new State("Q" + std::to_string(statenumber), false, false));
            transitions.push_back(new Transition(endstate, states.back(), epsilon));
            state = states.back();
            statenumber++;

            states.push_back(new State("Q" + std::to_string(statenumber), true, false));
            transitions.push_back(new Transition(state, states.back(), c));
            endstate = states.back();
            statenumber ++;
            if (temp != NULL){
                states.push_back(new State("Q" + std::to_string(statenumber), true, false));
                temp->setAccepting(false);
                endstate->setAccepting(false);
                transitions.push_back(new Transition(endstate, states.back(), epsilon));
                transitions.push_back(new Transition(temp, states.back(), epsilon));
                temp = NULL;
                endstate = states.back();
                statenumber++;
            }
        }
        else if (c == '+'){
            temp = endstate;
            endstate = previousstate;
        }
        else if (c == '*'){
            transitions.push_back(new Transition(endstate, state, epsilon));
            endstate->setAccepting(false);
            states.push_back(new State("Q" + std::to_string(statenumber), true, false));
            statenumber++;
            transitions.push_back(new Transition(endstate, states.back(), epsilon));
            endstate = states.back();
            transitions.push_back(new Transition(previousstate, endstate, epsilon));
        }
        else if (c == '('){
            brackets.push_back(previousstate);
            brackets.push_back(state);
        }
        else if (c == ')'){
            state = brackets.back();
            brackets.pop_back();
            previousstate = brackets.back();
            brackets.pop_back();
        }
    }

    for(char c : alphabet){
        std::cerr << c << ' ';
    }
    Automaton* automaton = new Automaton("eNFA", epsilon, states, transitions, alphabet);
    return automaton;
}


Automaton::Automaton(const std::string &type, char epsilon, const std::vector<State *> &states,
                     const std::vector<Transition *> &transitions, const std::vector<char> &alphabet) : type(type),
                                                                                                        epsilon(epsilon),
                                                                                                        states(states),
                                                                                                        transitions(
                                                                                                                transitions),
                                                                                                        alphabet(
                                                                                                                alphabet) {
    for(State* otherState : this->states){
        for(Transition* transition : this->transitions){
            otherState->addTransition(transition);
        }
    }

    bool test = false;
    for(State* state: this->states){
        if(state->isStarting()){
            test = true;
            if (this->startingState == NULL){
                this->startingState = state;
            }else{
                std::cerr << this->startingState->getName() << ' ' << state->getName() << std::endl;
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
    stream << "\tgraph [rankdir=LR]" << std::endl;
    stream << "\tDummy -> \"" << this->startingState->getName() << "\" ;" << std::endl;
    stream << "\tDummy [style=invis];" << std::endl;
    for(State* endstate : this->acceptStates){
        stream << "\t\"" << endstate->getName() << "\"[shape=doublecircle];" << std::endl;
    }
    for(Transition* transition : this->transitions){
        stream << "\t\"" <<transition->getBegin()->getName() << "\" -> \"" << transition->getEnd()->getName() << "\" [label=";
        if (transition->getInput() == this->epsilon){
            stream << "epsilon" <<"];" << std::endl;
        }
        else{
            stream << transition->getInput() <<"];" << std::endl;
        }
    }
    stream << "}" << std::endl;
}

State* Automaton::transition(State *state, std::string string) {
    State * newState = state;
    for(char c : string){
        if (std::find(alphabet.begin(), alphabet.end(), c) == alphabet.end()){
            throw(std::invalid_argument("only characters allowed from the alphabet"));
        }
        newState = newState->transition(c);
    }
    return newState;
}

std::vector<State *> Automaton::reachableStates() {
    std::vector<State*> states = {this->startingState};
    this->startingState->reacheableStates(states);
    return states;
}

void Automaton::deleteState(State *state) {
    //only deletes transitions FROM the state, all transitions TO the state are kept, so only delete eligible states
    if(state == this->startingState) //cannot delete starting state
        return;
    std::vector<Transition*> copyTransitions = this->transitions;
    for(Transition* transition : copyTransitions){
        if (transition->getBegin() == state){
            delete(transition);
            this->transitions.erase(std::find(this->transitions.begin(), this->transitions.end(), transition));
        }
    }
    this->states.erase(std::find(this->states.begin(), this->states.end(), state));
    if (std::find(this->acceptStates.begin(), this->acceptStates.end(), state) != this->acceptStates.end())
    this->acceptStates.erase(std::find(this->acceptStates.begin(), this->acceptStates.end(), state));

    delete(state);
}

void Automaton::deleteAllUnreachableStates() {
    std::vector<State*> reachablesStates = this->reachableStates();
    std::vector<State*> copyStates = this->states;
    for(State* state: copyStates){
        if (std::find(reachablesStates.begin(), reachablesStates.end(), state) == reachablesStates.end()){
            std::cerr << state->getName() << std::endl;
            this->deleteState(state);
        }
    }
}

std::vector<std::vector<State *>> Automaton::TableFilling(bool compare, std::ostream &stream) {
    if (!compare){
        this->deleteAllUnreachableStates();
    }
    std::vector<std::vector<bool>> table;
    for (int i = 1; i < states.size(); i++) {
        std::vector<bool> row = {};
        for (int j = 0; j < i; j++) {
            row.push_back(this->states[i]->isAccepting() != this->states[j]->isAccepting());
        }
        table.push_back(row);
    }



    bool success = true;
    while(success) {
        success = false;
        for (int i = 0; i < table.size(); i ++) {
            std::vector<bool> test = table[i];
            stream << states[i+1]->getName() << ' ';
            for (bool a : test) {
                stream << a << ' ';
            }
            stream << std::endl;
        }
        stream << "  ";
        for(int j = 0; j < table.back().size(); j++){
            stream << states[j]->getName() <<  ' ';
        }
        stream << std::endl<<std::endl;
        for (int i = 1; i < states.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (table[i - 1][j]) {
                    success = TableFillingStep(states[i], states[j], table) || success; //once succes => always success
                }
            }
        }
    }
    success = true;
    for (std::vector<bool> test : table) {
        for (bool a : test) {
            success = success && a;
            std::cerr << a << ' ';
        }
        std::cerr << std::endl;
    }
    if (success){
        return {{}};
    }else{
        std::vector<std::vector<State*>> ToBeMerged = {};
        std::vector<State*> subvector = {};
        for (int i = 1; i < this->states.size(); i ++){
            for(int j = 0; j < i; j++){
                if (!table[i-1][j]){
                    if (!(subvector.size() == 0 || std::find(subvector.begin(), subvector.end(), this->states[i]) != subvector.end() || std::find(subvector.begin(), subvector.end(), this->states[j]) != subvector.end())) {
                        ToBeMerged.push_back(subvector);
                        subvector = {};
                    }
                    if (std::find(subvector.begin(), subvector.end(), states[i]) == subvector.end())
                        subvector.push_back(states[i]);
                    if (std::find(subvector.begin(), subvector.end(), states[j]) == subvector.end())
                        subvector.push_back(states[j]);
                }
            }
        }
        ToBeMerged.push_back(subvector);
        for(std::vector<State*> test : ToBeMerged){
            for(State* a : test){
                std::cerr << a->getName() << ' ';
            }
            std::cerr << std::endl;
        }
        if (compare){
            return ToBeMerged;
        }
        for(std::vector<State*> states : ToBeMerged){
            State* stateToBeKept;
            if (std::find(states.begin(), states.end(), this->startingState) != states.end()){
                stateToBeKept = this->startingState;
            }else{
                stateToBeKept = states.front();
            }

            for(Transition* transition : this->transitions){
                if (std::find(states.begin(), states.end(), transition->getEnd()) != states.end()){
                    transition->setEnd(stateToBeKept);
                }
            }
            std::string name = "{";
            for(State* state: states){
                name += state->getName();
                if (state!= states.back()){
                    name += ", ";
                }
                if (state != stateToBeKept){
                    this->deleteState(state);
                }
            }
            name += "}";
            stateToBeKept->setName(name);
        }
    }
    return {{}};
}

bool Automaton::TableFillingStep(State *state1, State *state2, std::vector<std::vector<bool>> &table) {
    bool success = false;
    for (int a = 1; a < states.size(); a++){
        for(int b = 0; b < a; b++){
            if ((this->states[a] == state1 && this->states[b] == state2) || (this->states[a] == state2 && this->states[b] == state1)){
                continue;
            }
            for(Transition* transition1 : this->states[a]->getTransitions()){
                State* state = NULL;
                State* otherState = NULL;
                if (transition1->getEnd() == state1){
                    state = state1;
                    otherState = state2;
                }
                else if (transition1->getEnd() == state2){
                    state = state2;
                    otherState = state1;
                }
                if (state != NULL){
                    for(Transition* transition2: this->states[b]->getTransitions()){
                        if (transition2->getEnd() == otherState && !table[a-1][b] && transition1->getInput() == transition2->getInput()){
                            table[a-1][b] = true;
                            success = true;
                        }
                    }
                }
            }
        }
    }
    return success;
}

const std::vector<State *> &Automaton::getStates() const {
    return states;
}

const std::vector<Transition *> &Automaton::getTransitions() const {
    return transitions;
}

const std::vector<char> &Automaton::getAlphabet() const {
    return alphabet;
}

const std::vector<State *> &Automaton::getAcceptStates() const {
    return acceptStates;
}

const std::string &Automaton::getType() const {
    return type;
}

State *Automaton::getStartingState() const {
    return startingState;
}

void Automaton::toJSon(std::ostream &stream) {
    stream << "{" << std::endl;
    stream << "  \"type\": \"" << this->type << "\"," << std::endl;
    stream << "  \"alphabet\": [" << std::endl;
    for(char c : this->alphabet){
        stream << "    \"" << c << "\"" ;
        if (c != this->alphabet.back()){
            stream << ',';
        }
        stream << std::endl;
    }
    stream << "  ]," << std::endl;
    stream << "  \"states\": [" << std::endl;
    for(State* state: this->states){
        stream << "    {"<< std::endl;
        stream << "      \"name\": \"" << state->getName() << "\"," << std::endl;
        std::string start = state->isStarting() ? "true" : "false";
        stream << "      \"starting\": " << start << "," << std::endl;
        std::string accept = state->isAccepting() ? "true" : "false";
        stream << "      \"accepting\": " << accept << std::endl;
        stream << "    }";
        if (state != states.back()){
            stream << ',';
        }
        stream << std::endl;
    }
    stream << "  ]," << std::endl;
    stream << "\"transitions\": [";
    for(Transition* transition: this->transitions){
        stream << "    {" << std::endl;
        stream << "      \"from\": \"" << transition->getBegin()->getName() << "\"," << std::endl;
        stream << "      \"to\": \"" << transition->getEnd()->getName() << "\"," << std::endl;
        stream << "      \"input\": \"" << transition->getInput() << "\"" << std::endl;
        stream << "    }";
        if (transition != transitions.back()){
            stream << ',';
        }
        stream << std::endl;
    }
    stream << "  ]" << std::endl << "}" << std::endl;
}

std::vector<State *> Automaton::transitionNFA(State *state, std::string string) {
    std::vector<State*> states = {state};
    for(char c : string){
        std::vector<State*> temp = {};
        for(State* state1 : states){
            std::set_union(temp.begin(), temp.end(), state1->transitionNFA(c).begin(), state1->transitionNFA(c).end(), temp.begin());
        }
        states = temp;
    }
    return states;
}

std::vector<State *> Automaton::Eclose(State *state) {
    std::vector<State*> states = {state};
    state->Eclose(this->epsilon, states);
    return states;
}

void Automaton::toDFA() {
    if (type == "NFA"){
        std::vector<std::vector<State*>> newStates = {{this->startingState}};
        std::vector<State*> Globalstates = {new State((std::string)"{" + this->startingState->getName()+(std::string)"}", this->startingState->isAccepting(), true)};
        int length = 0;
        while (length != newStates.size()){
            length = newStates.size();
            std::cerr << length <<std::endl;
            std::vector<std::vector<State*>> tempstates = newStates;
            for(char c : this->alphabet){
                for(std::vector<State*> states : newStates  ){
                    bool starting = true;
                    bool accepting = false;
                    std::string name = "{";
                    std::vector<State*> temp = {};
                    for(State* state: states){
                        for(Transition* transition : state->getTransitions()){
                            if (transition->getInput() == c) {
                                if (std::find(temp.begin(), temp.end(), transition->getEnd()) == temp.end()) {
                                    temp.push_back(transition->getEnd());
                                    name += temp.back()->getName();
                                    name += ',';
                                    starting = starting && temp.back()->isStarting();
                                    accepting = accepting || temp.back()->isAccepting();
                                }
                            }
                        }
                    }
                    std::sort(temp.begin(), temp.end());
                    if (std::find(tempstates.begin(), tempstates.end(), temp) == tempstates.end()){
                        name.back() = '}';
                        tempstates.push_back(temp);
                        Globalstates.push_back(new State(name, accepting, starting));
                    }
                }
            }
            newStates = tempstates;
        }
        std::vector<Transition*> transitions = {};
        for(char c : this->alphabet){
            for(std::vector<State*> tempStates: newStates){
                std::vector<State*> temp = {};
                for(State* state: tempStates){
                    for(Transition* transition : state->getTransitions()){
                        if (transition->getInput() == c){
                            if (std::find(temp.begin(), temp.end(), transition->getEnd()) == temp.end())
                                temp.push_back(transition->getEnd());
                        }
                    }
                }
                std::sort(temp.begin(), temp.end());
                int index = std::distance(newStates.begin(), std::find(newStates.begin(), newStates.end(), tempStates));
                int index1 = std::distance(newStates.begin(), std::find(newStates.begin(), newStates.end(), temp));

                transitions.push_back(new Transition(Globalstates[index],Globalstates[index1], c));
            }
        }
        Automaton a = Automaton("DFA", this->epsilon, Globalstates, transitions, alphabet);
        this->acceptStates = a.getAcceptStates();
        this->startingState = a.getStartingState();
        this->states = Globalstates;
        this->transitions = transitions;
        this->type = "DFA";
    }
}




