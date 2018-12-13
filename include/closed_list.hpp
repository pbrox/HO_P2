//Definition of closed list -- Only inline functions

#ifndef CLOSED_L
#define CLOSED_L

#include <utility>
#include "t_state.hpp"

struct compare { //For the set purposes as the parent is irrelevant for our comparations
    bool operator() (const astar_node &a, const astar_node &b) const{
    	return a.first < b.first;
    }
};

class closed_list{

	public:

		//constructor
		closed_list(){}
		//Destructor
		~closed_list(){}

		//inseting an element
		bool push(astar_node to_insert){return (list.insert(to_insert)).second;}

		//Checking if an element belongs
		bool belongs(astar_node to_check){return list.find(to_check) != list.end();}

	private:

		//Set itself
		std::set<astar_node, compare> list;

};
#endif 

//TO DO problem cost and heuristic may do different states!!!!