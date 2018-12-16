#include "open_list.hpp"


struct compare_g { //Compare function to order by descendent order (pop last will be max) each buck
    bool operator() (const astar_node &a, const astar_node &b) const{
    	return a.first.get_g() < b.first.get_g();
    }
};

//Adding a new state
void open_list::push(const astar_node & state){

	//First obtain bucket id (Heutistic value)
	int i = state.first.get_f();

	//then obtain the place where it should be inserted
	auto iterator = std::upper_bound(container[i].begin(), container[i].end(), state, compare_g());

	//insert the node
	container[i].insert(iterator, state);

	//Add one to the state counter
	number_nodes++;

	//Check if we have to update the index and update
	minimum = (i < minimum) ? i : minimum;

}

//Extracting next state
astar_node open_list::pop(){

	//Locate and obtain value
	auto to_return = container[minimum].back();

	//Remove element
	container[minimum].pop_back();
	number_nodes--;

	//Check if we have to update the minimum index
	if (container[minimum].size() != 0) return to_return; //We just return because if the vector still have contents is still minimum
	if (number_nodes == 0){ //If we have no more nodes we just set the minimum to the first to avoid problems
		minimum = container.size(); //Maximum value to avoid problems
		return to_return;
	}

	//Only if necessary check new minimum
	for(size_t i = minimum + 1; i < container.size(); ++i){ //This is always findeng something because if there are not more node we have already exited
		if(container[i].size() != 0){ //When we find a node with elements
			minimum = i; //Set the minimum to it
			break; //stop searchinf
		}
	}

	//Return state
	return to_return;
}
