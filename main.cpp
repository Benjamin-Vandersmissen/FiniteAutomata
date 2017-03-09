#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../DFA.json");
    json_parser parser(file);
    std::ofstream outputfile("../output.dot");
    Automaton* automaton = parser.getAutomaton();
    automaton->toDotFormat(outputfile);
    outputfile.close();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}