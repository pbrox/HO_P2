//Definition of open list
#ifndef OPEN_L
#define OPEN_L

#include "t_state.hpp"
#include <vector>
#include <array>

/*
	TO OD:
	* Fix type (Astar node)
	* Insert sorted!
	* Implementation!

*/

class open_list(){

	public:
		//Constructor
		open_list(int max_heuristic);
		~open_list{}

		//Adding a new state
		void push(astar_node state);

		//Extracting next state
		astar_node pop();

	private:

		//Bucket inside it is the new value to pop
		int minimum;

		//Container (Data structure) itself
		std::array<std::vector<astar_node>> container;

		//Returns the new minimum index after pushing
		int new_min();
};

#endif
//TO DO: implementation, what to do with empty?¿?