//Definition of open list
#ifndef OPEN_L
#define OPEN_L

#include "t_state.hpp"
#include <vector>
#include <array>
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort

class open_list{

	public:
		//Constructor
		open_list(int max_heuristic_val):  minimum(max_heuristic_val), number_nodes(0), container(max_heuristic_val) {}
		~open_list() {}

		//Adding a new state
		void push(const astar_node & state);

		//Extracting next state
		astar_node pop();

		//Check emptyness
		bool is_empty() const {return number_nodes == 0;}

	private:

		//Bucket inside it is the new value to pop
		int minimum;

		//Tracks the current inserted nodes in al the structure
		int number_nodes;

		//Container (Data structure) itself
		std::vector<std::vector<astar_node>> container;
};

#endif


