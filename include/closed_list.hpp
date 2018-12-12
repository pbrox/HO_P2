//Definition of closed list -- Only inline functions

#ifndef CLOSED_L
#define CLOSED_L

#include <utility>
#include "t_state.hpp"

class closed_list{

	public:

		//constructor
		open_list(){}
		//Destructor
		~closed_list(){}

		//inseting an element
		bool push(t_state to_insert){return (list.insert(to_insert)).second;}

		//Checking if an element belongs
		bool belongs(t_state to_check){return list.find(to_check) != list.end();}

	private:

		//Set itself
		std::set<t_state> list;

};
#endif 

//TO DO problem cost and heuristic may do different states!!!!