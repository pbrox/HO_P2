#include "t_state.hpp"
#include "maze.hpp"

t_state::~t_state(){

}
//Returns the position in the state
void t_state::get_position(int &x, int &y) const{

	x = AL_position.first;
	y = AL_position.second;

}


/*
	*	This method returns true if the choose moovement is legal in the given map, i.e,
	*	
	*		-> Resulting position is inside the map
	*		-> Resulting position is not a wall
	*   	-> Resulting position is not a snake (this two will mean that it is free)
	*   	-> if resulting has a stone, if you can push it 
	*   	-> If resulting row has a snake, a stone or a wall is protecting him

*/

bool t_state::can_move(moves move, const Maze &map){

	bool is_stone = false; //used below

	//Obtaining new position
	auto new_pos = next_pos(move, {});

	//Checking basic premises (inside limits, no wall or snake (free))
	if(new_pos.first >= map.ydim || new_pos.first < 0 || new_pos.second >= map.xdim || new_pos.second < 0
		|| map(new_pos.first, new_pos.second) != empty) return false;

	//Checking stone and checking that move this stone is allowed
	if(std::find(stones.begin(), stones.end(), new_pos) != stones.end()){
		is_stone = true;
		auto new_stone_pos = next_pos(move, new_pos);
		//Check if new stone postition is inside the map, is empty and no stone is on int (DOUBT!) 
		if(new_stone_pos.first >= map.ydim || new_stone_pos.first < 0 
		|| new_stone_pos.second >= map.xdim || new_stone_pos.second < 0
		|| map(new_stone_pos.first, new_stone_pos.second) != empty
		|| std::find(stones.begin(), stones.end(), new_stone_pos) != stones.end()) return false;
	}

	//Checking row for snakes
	/*	
		*	The procedure is to go first to the right and then to the left on the row, until:
		*		-> A wall, a stone or limit reached -> position allowed
				-> A snake is found -> position illegal

		This loop checking is only executed if the special case of move to the right && stone at right (or the same with left),
		this is because as we are not changing the stones (just cckecking), new position will have a stone, and that stone is 
		moved to the right (or left), so this part is still protected, visually,

			[][][AL][O][][S]
					 ^
					 New pos if move == right
			And the reuslting state if the move is applied will be

			[]][][][AL][O][S]

			So checking new_pos and move we can assure that this side is protected and therefore skip the loop
	*/
	//To the right
	if(!(is_stone && move == right)){	
		for(int i = new_pos.second + 1; i < map.xdim; ++i){
			//If snake found cannot move
			if(map(new_pos.first, i) == snake) return false;
			//If it is a stone or a wall allowed
			if((std::find(stones.begin(), stones.end(), std::make_pair(new_pos.first, i)) != stones.end())
				|| map(new_pos.first, i) == wall) break; 
		}
	}

	//To the left
	if(!(is_stone && move == left)){	
		for(int i = new_pos.second - 1; i >= 0; --i){
			//If snake found cannot move
			if(map(new_pos.first, i) == snake) return false;
			//If it is a stone or a wall allowed
			if((std::find(stones.begin(), stones.end(), std::make_pair(new_pos.first, i)) != stones.end())
				|| map(new_pos.first, i) == wall) break; 
		}
	}

	//If all test passed
	return true;
}

/*			 WARNING! 
	This move method assume
	that the movement is correct, un-
	defined behaviour if the movement
	is not allowed! This can be chec-
	ked beforehand with can_move.
*/

//returns the state generated executing operation move
t_state t_state::move(moves move, const Maze &map){
	
	//Obtaining an element to copy
	auto new_state = *this;

	//Obtaining new position
	auto new_pos = next_pos(move, {});

	//Updating fields of the new state
	//If new position is a key, remove it
	if(new_state.keys.find(new_pos) != new_state.keys.end()) new_state.keys.erase(new_pos);

	//If necessary move stones and update cost to stones
	//searching new position in stones vector
	auto stones_iterator = std::find(new_state.stones.begin(), new_state.stones.end(), new_pos);
	if( stones_iterator != new_state.stones.end()){
		//Move the stone
		*stones_iterator = next_pos(move, *stones_iterator);
		//Updating the cost
		new_state.g += 4;
	} else new_state.g += 2; //If the AL does not need to move the stone the cost is only increased by 2

	//Setting the new AL position
	new_state.AL_position = new_pos;

	//computing the new heuristics
	new_state.heuristic_v = new_state.heuristic(used_heuristic, std::make_pair(map.goal_row(), map.goal_column()));

	//Returning new state
	return new_state;
}

//Heuristic function of the current state
int t_state::heuristic(heuristic_funcs choosen, const std::pair<int,int> &goal){

	//Swiches between availabe heuristics and returns choosen one
	switch (choosen){
		case h_default:
			return default_h();
		case min_k_mahattan:
			return min_mahattan_key(goal);
		default:
			return -1; //Returns a negative vaue if error
	}
}

//This method returns true if the state is final in the given map
bool t_state::is_final(const Maze &map){

	//Returning true if the keys set is empty and the AL_position is the map goal
	return (keys.size() == 0 && map.isGoal(AL_position.first,AL_position.second));
}

//Function returning next position (no checks)
std::pair<int,int> t_state::next_pos(moves move, std::optional<std::pair<int,int>> from){

	int curr_i , curr_j; //Getting current position of the AL
	//If no optional parameter, use actual position
	if(!from.has_value()){curr_i = AL_position.first; curr_j = AL_position.second;}
	else{curr_i = from->first, curr_j = from->second;}//Else use given parameter
	

	//returning the new position without checks
	switch (move){
		case up: //moving the before row
			return std::make_pair(curr_i-1,curr_j);
		case down: //moving the next row
			return std::make_pair(curr_i+1, curr_j);
		case left: //moving the before column
			return std::make_pair(curr_i, curr_j-1);
		case right: //Moving the next column
			return std::make_pair(curr_i, curr_j+1);
		default:
			return std::make_pair(-1,-1); //Impossible possition in case bad argument
	}

	//No need returning value since all cases covered
}

//Default heuristic function
int t_state::default_h(){
	return 0;
}

//Heuristic returning the minimum manhattan distance to a key or the mh to the goal in case of empty
int t_state::min_mahattan_key(const std::pair<int,int> &goal){

	//If only the goal is to reach (no keys left) retun mh to the goal
	if(keys.size() == 0) return std::abs(AL_position.first - goal.first) + std::abs(AL_position.second - goal.second);

	//Compute MH to each key
	int sum = 0;
	for(auto & i : keys) {
		//Compue mh distance to key
		int mh_k = std::abs(AL_position.first - i.first) + std::abs(AL_position.second - i.second);
		//If it is less than the minimum change the minimum
		sum += mh_k;
	}

	return sum;
}

//Equal operator
bool t_state::operator== (const t_state &rhs) const{

	//Check each value but cost, heuristic and heuristic function
	if(rhs.AL_position != this->AL_position) return false;
	if(rhs.keys != this->keys) return false; //This works because set is an ordered structure and if they contain same elements they will be equal

	//Now in stones (as we do not care about order here)
	//Now check that every element of one is in the other
	for(auto &i : rhs.stones){
		if(std::find(this->stones.begin(), this->stones.end(),i) == this->stones.end()) return false;
	}

	//If all conditions for equal
	return true;

}

//overloading < operator for states, Lexicographical
bool t_state::operator< (const t_state &rhs) const{

	//First comparate the AL_position, if they are not equal
	if(AL_position != rhs.AL_position) return AL_position < rhs.AL_position;

	//If positions are the same, let's check vectors
	//sort them because are equal if they have same elements  regardess the positions
	auto v1 = this->stones, v2 = rhs.stones;
	std::sort(v1.begin(), v1.end());
	std::sort(v2.begin(), v2.end());

	//Comparing lexicogafical (bare in mind they have aways same length) eemet by element
	for( size_t  i= 0; i < v1.size(); ++i) if(v1[i] != v2[i]) return v1[i] < v2[i];

	//Finaly, we go with the keys
	if(keys != rhs.keys) return keys < rhs.keys;
	
	//If this check is finished, then states are equa
	return false;
	
}

//printing the state, for debug purposes
void t_state::print(){

	std::cout << "[STATE INFO]\n";
	std::cout << "AL --> ROW [" << AL_position.first << "] COL [" << AL_position.second << "]\n";
	std::cout << "KEYS SET { \n";
	for(auto &i : keys){
		std::cout << "\tKEY ->[" << i.first << " ," << i.second << "]\n";
	}
	std::cout << "}\n";
	std::cout << "STONES SET { \n";
	for (auto &i : stones){
		std::cout << "\tSTONE ->[" << i.first << " ," << i.second << "]\n";
	}
	std::cout << "}\n";
	std::cout << "G = " << g << " HEURISTIC_VAL = " << heuristic_v << '\n';
	std::cout << "F FUNCTION is " << g+heuristic_v << '\n' << "[END STATE]\n";

}