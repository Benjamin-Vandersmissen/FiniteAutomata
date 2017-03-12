#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../test 1.json");
    json_parser parser(file);
    std::ofstream outputfile("../output.dot");
    Automaton* automaton = parser.getAutomaton();
//    automaton->deleteAllUnreachableStates();
//    std::cerr << automaton->transition(automaton->getState("A"), "100")->getName() << std::endl;
    for(State* state: automaton->reachableStates()){
        std::cerr << state->getName() << std::endl;
    }
    automaton->TableFilling();
    automaton->toDotFormat(outputfile);
    outputfile.close();
    return 0;
}