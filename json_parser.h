//
// Created by Benjamin on 06/03/2017.
//

#ifndef PARSER_JSON_PARSER_H
#define PARSER_JSON_PARSER_H

#include <iostream>
#include "Automaton.h"

class json_parser {
private:
    std::istream& in;
    Automaton* automaton;
protected:
    State* readState();
    Transition* readTransition();
public:
    json_parser(std::istream &stream);
    void skip_whitespace();
    void readAutomaton();
    char getChar();
    std::string readQuotedString();
    char readQuotedChar();
    void expectChar(char c);
    std::vector<char> readAlphabet();
    std::vector<State*> readStates();
    std::vector<Transition*> readTransitions();
    void expectQuotedString(std::string expected);
    bool getBool();
};


#endif //PARSER_JSON_PARSER_H
