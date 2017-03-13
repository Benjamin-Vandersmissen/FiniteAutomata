#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../DFA1.json");
    Automaton* a1 = parse(file);
    file.close();
    file.open("../DFA2.json");
    Automaton* a2 = parse(file);
    std::ofstream output("../DFA1.dot");
    a1->toDotFormat(output);
    output.close();
    output.open("../DFA2.dot");
    a2->toDotFormat(output);
    output.close();
    if (areEquivalent(a1,a2)){
        std::cerr << "TEST" << std::endl;
    }
    output.open("../test.dot");
    a1->TableFilling(false, std::cerr);
    a1->toDotFormat(output);
    output.close();
    return 0;
}