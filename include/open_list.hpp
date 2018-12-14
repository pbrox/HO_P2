//Definition of open list
#ifndef OPEN_L
#define OPEN_L

#include "t_state.hpp"
#include <vector>
#include <array>

/*
	TO OD:
	* Insert sorted!
	* Implementation!

*/

class open_list(){

	public:
		//Constructor
		open_list(int max_heuristic);
		~ open_list {}

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
		std::array<std::vector<astar_node>> container;

		//Returns the new minimum index after pushing
		int new_min();
};

#endif
//TO DO: implementation

/*
Remarks: ->1 operador menor funcion compare para upper boud
2 -> upper boud
3-> insert upper bound
and be happy 
*/