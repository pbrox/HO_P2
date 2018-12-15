#include <iostream>
#include <fstream>
#include <string>
#include "s_agent.hpp"
#include "t_state.hpp" //For heuristic funcs definitions

//Function that writes help page in std::out
void print_help();
//function that matches each posible heuristic input into an heuristic_funcs type
heuristic_funcs parse_heuristic(std::string to_use);

int main(int argc, char ** argv){

	//Parsing arguments
	if(argc < 3){ //If incorrect
		if(argc == 2 && std::string(argv[1]) == "--help"){ //Either help
			print_help(); //Print help
			return 0;
		}

		//Or bad call, in that case inform the user
		std::cout << "Incorrect arguments, use AstarPaganitzu --help to view help\n";
		return -1;
	}
	
	//Create an agent eith the given map and heuristic functions
	s_agent c_agent(argv[1], parse_heuristic(std::string(argv[2])));
	//Execute the Astar inside the agent
	c_agent.do_search();

	//Output agent statistics and results to file
	std::fstream file_to_write;

	//For output file
	file_to_write.open(std::string(argv[1])+".output", std::fstream::out); //Opens the file with .output extension
	if(!file_to_write.is_open()){ //Does nothing if the opening was impossible
		std::cout << "Error opening .output file!\n";
		return -1;
	}
	c_agent.print_result(file_to_write); //Write results to file
	file_to_write.close(); //Close the file

	//For statistics file
	file_to_write.open(std::string(argv[1])+".statistics", std::fstream::out); //Opens the file with .statistics extension
	if(!file_to_write.is_open()){ //Does nothing if the opening was impossible
		std::cout << "Error opening .output file!\n";
		return -1;
	}
	c_agent.print_statistics(file_to_write); //Write statistics to file
	file_to_write.close(); //Close the file

	//Debug -- erase of the final vesion
	c_agent.print_result(std::cout);
	c_agent.print_statistics(std::cout);
	return 0;

}

//Function that writes help page in std::out
void print_help(){
	std::cout << "Astar Paganitzu Help\n";
	std::cout << "Usage AstarPaganitzu map heuristic\n";
	std::cout << "\tmap: Path to the map file\n";
	std::cout << "\theuristic: heuristic to use, available are:\n";
	std::cout << "\t\t default_heuristic: returns 1, making djsktra\n";
	std::cout << "\n use AstarPaganitzu --help to see this page\n";
}

//function that matches each posible heuristic input into an heuristic_funcs type
heuristic_funcs parse_heuristic(std::string to_use){
	if (to_use == "default_heuristic") return h_default;
	else return h_default;
}


/*
	To do:
	 
	 * Main
	 * HELP

*/