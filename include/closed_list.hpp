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
		bool push(const astar_node  & to_insert){return (list.insert(to_insert)).second;}

		//Checking if an element belongs (0 cast to false, 1 to true, no more possible vaues of count for a set)
		bool belongs(const astar_node & to_check) const {return list.count(to_check);}

	private:

		//Set itself
		std::set<astar_node, compare> list;

};
#endif 
