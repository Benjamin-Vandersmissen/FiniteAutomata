#include <iostream>
#include <fstream>
#include "json_parser.h"

int main(int argc, char* argv[]) {
    std::ifstream file("../eNFA.json");
    std::ofstream DOTfile("./eNFA.dot");

    Automaton* automaton = parse(file);
    automaton->toDotFormat(DOTfile);
    for(State* state: automaton->getStates()){
        std::cerr << state->getName() << " ECLOSE: ";
        std::vector<State*> states = automaton->Eclose(state);
        for(State* s : states){
            std::cerr << s->getName() << ' ';
        }
        std::cerr << std::endl;
    }
    return 0;
}