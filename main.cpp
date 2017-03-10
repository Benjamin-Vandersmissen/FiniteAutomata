#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../test.json");
    json_parser parser(file);
    std::ofstream outputfile("../output.dot");
    Automaton* automaton = parser.getAutomaton();
    automaton->toDotFormat(outputfile);
    outputfile.close();
    std::cerr << automaton->transition(automaton->getState("A"), "100")->getName() << std::endl;
    return 0;
}