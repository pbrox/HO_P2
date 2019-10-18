#ifndef AGENT
#define AGENT
//Definition of s_agent class

#include <string> //Parsing input ?¿¿? (remove!)
#include <chrono>
#include "maze.hpp"
#include "t_state.hpp"
#include "closed_list.hpp"
#include "open_list.hpp"

inline static constexpr int max_h = 1024;

class s_agent{

	public:

		//Constructs the needed srtucts to perfom Astar
		s_agent(std::string path, heuristic_funcs h_funct): map(path,h_funct) {}

		//Function to tell the agent to perform A* starting from initial state of Map
		void do_search();

		//Functions to print statistics and path
		bool print_result(std::ostream & flux_to);
		bool print_statistics(std::ostream & flux_to);

		//To know if solution found
		bool get_sol_state(){return solution;}

	private:

		//Contains a Map (with constains an initial state) constant after initialization
		const Maze map;

		//Data for result
		std::vector<std::pair<int,int>> path_end; 

		//Data for statistics
		std::chrono::milliseconds execution_time; //ms
		int total_cost; 
		int total_expansions = 0;

		//Bool to kno if the current agent has already been executed
		bool executed = false;
		//Bool to know if there is solution
		bool solution = false; //starts as false

		//Returns possible children for a given parent
		std::vector<astar_node> expand(const astar_node & current);
};

#endif