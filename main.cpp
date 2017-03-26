#include <iostream>
#include <fstream>
#include "json_parser.h"

int main(int argc, char* argv[]) {
	std::ifstream file;
	std::ofstream DOTfile;
	if (argc == 2){
		std::string filename = (std::string)(argv[1]);
		file.open(argv[1]);
		DOTfile.open(filename.substr(0, filename.find(".json"))+".dot");
		Automaton* automaton = parse(file);
        automaton->toDFA();
        automaton->toDotFormat(DOTfile);
	}
//
//
//
//    Automaton* automaton = parse(file);
//    automaton->toDotFormat(DOTfile);
//    for(State* state: automaton->getStates()){
//        std::cerr << state->getName() << " ECLOSE: ";
//        std::vector<State*> states = automaton->Eclose(state);
//        for(State* s : states){
//            std::cerr << s->getName() << ' ';
//        }
//        std::cerr << std::endl;
//    }
    return 0;
}