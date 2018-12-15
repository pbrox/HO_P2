#include <iostream>
#include <string>
#include "s_agent.hpp"
#include "t_state.hpp" //For heuristic funcs definitions


int main(int argc, char ** argv){
	
	s_agent c_agent(argv[1], h_default);
	c_agent.do_search();
	c_agent.print_result(std::cout);
	c_agent.print_statistics(std::cout);
	return 0;

}



/*
	To do:
	 
	 * Main
	 * HELP

*/