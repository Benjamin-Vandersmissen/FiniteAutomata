#include <iostream>
#include <fstream>
#include "json_parser.h"

int main(int argc, char* argv[]) {
	std::ifstream file;
	std::ofstream DOTfile;
    std::string args[argc];
    for(int i = 0; i < argc; i++){
        args[i] = (std::string)argv[i];
    }
	if (argc == 2 || (argc == 4 && args[2] == "-o")){
		std::string filename = args[1];
		file.open(filename);
        if (argc == 2)
		    DOTfile.open(filename.substr(0, filename.find(".json"))+".dot");
        else
            DOTfile.open(args[3].substr(0,args[3].find(".dot")) + ".dot");
        Automaton* automaton = parse(file);
        automaton->toDotFormat(DOTfile);
	}
    else if ((args[1] == "-DFA" or args[1] == "-toDFA")){
        std::string filename;        
        if (args[2] == "--all"){
           filename = args[3];
        }
        else{
           filename = args[2];
        }
        file.open(filename);
        Automaton* automaton = parse(file);
        if(args[2] == "--all"){
            DOTfile.open(filename.substr(0, filename.find(".json"))+".dot");
            automaton->toDotFormat(DOTfile);
            DOTfile.close();
        }
        automaton->toDFA();
        std::ofstream JSONfile(filename.substr(0, filename.find(".json"))+" - DFA.json");
        automaton->toJson(JSONfile);
        JSONfile.close();
        DOTfile.open(filename.substr(0, filename.find(".json")) + " - DFA.dot");
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
