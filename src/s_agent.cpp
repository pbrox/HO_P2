#include "s_agent.hpp"

//Function to tell the agent to perform A* starting from initial state of Map
void s_agent::do_search(){

	//A: Prepating steps, creating and initializating needed structures

	//oepened and closed list
	open_list opened(max_h);
	closed_list closed;

	//Inserting First node, composed of an initial state and add empty path
	auto initial = *map.initial_state;
	opened.push(std::make_pair(initial, std::vector<std::pair<int,int>>(0)));

	//B: Astar algorithm

	//Measuring time
	auto start = std::chrono::high_resolution_clock::now();

	while(!opened.is_empty()){ //While we have opened nodes (when we expand solution also stops by using break)

		//obtain the first element to expand
		auto to_expand = opened.pop();

		//check if the state is final
		if(to_expand.first.is_final(map)){
			solution = true; //Set the flag solution to found
			total_expansions++; //Because we are actually expanding it
			//Set the parameter to the last expanded state(goal)
			path_end = to_expand.second;
			//Getting solution cost
			total_cost = to_expand.first.get_g();
			//Adding itself to path
			path_end.push_back(to_expand.first.get_position());
			break; //End Astar
		}

		//Check if it is already expanded (this is done by trying to insert in the closed, if already exists it fails)
		if(closed.push(to_expand)){ //Only if the node was not already expanded, we insert in in the closed list

			//Generate childs
			std::vector<astar_node> children = expand(to_expand);

			//Insert childs (only if they are not already expanded)
			for(auto &i : children) if(!closed.belongs(i)) opened.push(i);
		}

	}

	//Measuring time to compute elapsed
	auto finish = std::chrono::high_resolution_clock::now();

	//C: Outputs obtention
	
	//End flag
	executed = true;

	//Getting the time measurement in miliseconds
	std::chrono::duration<double> elapsed = finish - start;
	execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);



}

//Returns possible children for a given parent
std::vector<astar_node> s_agent::expand(const astar_node & current){

	//Generate the empty vecto
	std::vector<astar_node> to_return;

	//Generate the base state adding the father position to the path, NOT CHANGING ANCTUAL STATE BY NOW
	auto mid_state = current;
	mid_state.second.push_back(current.first.get_position()); //Adding father

	//Now for each possible movement, check if it possible withim the Maze, if so, insert it
	//Up
	if(mid_state.first.can_move(up, map)) to_return.push_back(std::make_pair(mid_state.first.move(up, map),mid_state.second));

	//Down
	if(mid_state.first.can_move(down, map)) to_return.push_back(std::make_pair(mid_state.first.move(down, map),mid_state.second));

	//Left
	if(mid_state.first.can_move(left, map)) to_return.push_back(std::make_pair(mid_state.first.move(left, map),mid_state.second));

	//Right
	if(mid_state.first.can_move(right, map)) to_return.push_back(std::make_pair(mid_state.first.move(right, map),mid_state.second));

	//Satistics adding one to expanded node
	total_expansions++;

	//returning the vector of suitable children
	return to_return;
}

//Function to print path
bool s_agent::print_result(std::ostream & flux_to){
	
	if(!executed) return false; //Not done if not executed

	if(!solution){ //FIXME: Esto duda 
		flux_to << "Search Agent was not able to find a solution for your problem\n";
		return true;
	}

	//Print path
	flux_to << "{ ";
	for(auto &i : path_end) flux_to << "(" << i.first << "," << i.second << ") "  << std::string((&i != &path_end.back()) ? "-> " : "");
	flux_to << "}\n";
	
	//returning success
	return true;
}

//Function to print statistics
bool s_agent::print_statistics(std::ostream & flux_to){

	if(!executed) return false; //Not done if not executed

	//Statistics for both solution and not PREGUNTAR, time
	flux_to << "Overall running time: " << execution_time.count() << " miliseconds\n";

	//Stats when solution is found
	if(solution){
		//cost
		flux_to << "Total cost: " << total_cost << '\n';
		//Solution length
		flux_to << "Step length: " << path_end.size() << '\n';
	}

	//Expansions, for both cases DUDA
	flux_to << "Number of expansions: " << total_expansions << '\n';

	return true;
}