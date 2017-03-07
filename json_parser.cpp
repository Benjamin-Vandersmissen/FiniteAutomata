//
// Created by Benjamin on 06/03/2017.
//

#include "json_parser.h"

json_parser::json_parser(std::istream &stream) : in(stream) {
    in.clear();
    readAutomaton();
}

void json_parser::skip_whitespace() {
    if(in.eof()){ //end of file reached
        return;
    }
    while(!in.eof()){
        if(isspace(in.peek())){
            in.get();
        }
        else{
            return;
        }
    }
}

void json_parser::readAutomaton() {
    skip_whitespace();
    char c = getChar();
    if(c != '{'){
        throw(std::invalid_argument((std::string)"Expected { but got " + c + (std::string)" instead"));
    }
    skip_whitespace();
    expectQuotedString("type");
    std::string type;
    expectChar(':');
    type = readQuotedString();
    expectChar(',');
    std::cerr << type << std::endl;

    expectQuotedString("alphabet");
    std::vector<char> alphabet;
    expectChar(':');
    alphabet = readAlphabet();
    expectChar(',');

    char eps;
    if(type == "eNFA"){
        expectQuotedString("eps");
        expectChar(':');
        eps = readQuotedChar();
        expectChar(',');
        std::cerr << eps << std::endl;
    }
    expectQuotedString("states");
    std::vector<State*> states;
    expectChar(':');
    states = readStates();

    expectChar(',');
    expectQuotedString("transitions");
    std::vector<Transition*> transitions;
    expectChar(':');
    transitions = readTransitions();
    expectChar('}');

    Automaton* automaton = new Automaton(type,eps, states, transitions, alphabet);
    this->automaton = automaton;
}

char json_parser::getChar() {
    return (char) in.get();
}

std::string json_parser::readQuotedString() {
    char quote;
    skip_whitespace();
    char c = getChar();
    if(c == '\'' or c == '"'){
        quote = c;
    }else{
        throw(std::invalid_argument((std::string)"Expected \" but got " + c + (std::string)" instead"));
    }
    std::string string;
    c = getChar();
    while(c != '\n'){
        string += c;
        c = getChar();
        if (c == quote){
            break;
        }
    }
    if (c == '\n'){
        throw(std::invalid_argument((std::string)"Expected " + quote + (std::string)" but got none"));
    }
    return string;
}

void json_parser::expectChar(char c) {
    skip_whitespace();
    char c2 = getChar();
    if(c2 != c){
        throw(std::invalid_argument((std::string)"Expected " + c + (std::string)" but got " + c2 + (std::string)" instead"));
    }
}

std::vector<char> json_parser::readAlphabet() {
    expectChar('[');
    std::vector<char> alphabet;
    while(true){
        char c = readQuotedChar();
        alphabet.push_back(c);
        skip_whitespace();
        if((char) in.peek() != ']'){
            expectChar(',');
            skip_whitespace();
        }
        else{
            getChar();
            break;
        }
    }
    return alphabet;
}

std::vector<State *> json_parser::readStates() {
    expectChar('[');
    std::vector<State*> states;
    while(true) {
        State *state = readState();
        states.push_back(state);
        skip_whitespace();
        if((char) in.peek() != ']'){
            expectChar(',');
            skip_whitespace();
        }else{
            getChar();
            break;
        }
    }
    return states;
}

State *json_parser::readState() {
    expectChar('{');
    std::string string;
    expectQuotedString("name");
    expectChar(':');
    std::string name = readQuotedString();
    expectChar(',');
    expectQuotedString("starting");
    expectChar(':');
    bool starting = getBool();
    expectChar(',');
    expectQuotedString("accepting");
    expectChar(':');
    bool accepting = getBool();
    expectChar('}');
    State * state = new State(name, accepting, starting);
    return state;
}


std::vector<Transition *> json_parser::readTransitions() {
    expectChar('[');
    std::vector<Transition*> transitions;
    while(true){
        Transition* transition = readTransition();
        transitions.push_back(transition);
        skip_whitespace();
        if(in.peek() != ']'){
            expectChar(',');
            skip_whitespace();
        }
        else{
            getChar();
            break;
        }
    }
    return transitions;
}

Transition *json_parser::readTransition() {
    expectChar('{');
    expectQuotedString("from");
    std::string from;
    expectChar(':');
    from = readQuotedString();

    expectChar(',');
    expectQuotedString("to");
    std::string to;
    expectChar(':');
    to = readQuotedString();

    expectChar(',');
    expectQuotedString("input");
    char input;
    expectChar(':');
    input = readQuotedChar();

    expectChar('}');
    Transition* transition = new Transition(from, to, input);
    return transition;
}

void json_parser::expectQuotedString(std::string expected) {
    std::string string = readQuotedString();
    if(string != expected){
        throw(std::invalid_argument((std::string)"Expected \"" + string + (std::string)"\" but got \"" + expected + "\""));
    }
}

bool json_parser::getBool() {
    skip_whitespace();
    std::string s;
    s += getChar();
    s += getChar();
    s += getChar();
    s += getChar();
    if (s == "true"){
        return true;
    }
    s += getChar();
    if (s == "false"){
        return false;
    }
    throw(std::invalid_argument((std::string)"Expected true or false but got " + s + (std::string)" instead"));
}

char json_parser::readQuotedChar() {
    skip_whitespace();
    std::string string = readQuotedString();
    if(string.size() == 1){
        return string[0];
    }
    else{
        throw(std::invalid_argument((std::string)"Expected a quoted char but got " + string + (std::string)" instead"));
    }
}


