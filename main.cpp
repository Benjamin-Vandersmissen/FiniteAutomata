#include <iostream>
#include "json_parser.h"
#include <fstream>

int main() {
    std::ifstream file("../DFA.json");
    json_parser parser(file);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}