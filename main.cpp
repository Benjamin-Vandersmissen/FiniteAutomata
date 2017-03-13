#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../DFA1.json");
    Automaton* a1 = parse(file);
    file.close();
    file.open("../DFA2.json");
    Automaton* a2 = parse(file);
    if (areEquivalent(a1,a2)){
        std::cerr << "TEST" << std::endl;
    }
    return 0;
}