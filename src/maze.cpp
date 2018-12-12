#include "maze.hpp"
#include "t_state.hpp"
//Exception for constructor
class constExc: public std::runtime_error
{
    public:
        constExc(std::string const& err_str):
            std::runtime_error(err_str)
        {}
};


//Maze class implementation

//Destructor
Maze::~Maze(){

}

//Constructor
Maze::Maze(std::string path){

		//Parameters needed for new state (initial)
		std::set<std::pair<int,int>> keys;
		std::vector<std::pair<int,int>> stones;
		std::pair<int,int> al;  

		//File opening
		std::ifstream inputMap;
		inputMap.exceptions (std::ifstream::badbit );

		try{
			inputMap.open (path, std::ios::in);

			//Start parsing
			std::string aux_line; //Used to obtain current line of the file
			//Get first line
			std::getline(inputMap, aux_line);
			if(!aux_line.length()) throw constExc( "empty maze!\n" );

			//Assigning first dimensons
			xdim = aux_line.length();
			ydim = 0;

			//Intializing goal and init
			goal[0] = -1;

			//parsing
			do{
				//Cheking line length
				if(aux_line.length() != (long unsigned int)(xdim)) throw constExc( "Bad dimensions!\n" );
				//Reserving new part of vector
				maze_grid.reserve(xdim + maze_grid.size());

				cell_type aux_cell;
				//for each character of the maze row
				for(int i= 0; i < xdim; ++i){
					switch (aux_line[i]){
						case '%': //Non traversable
							aux_cell = wall;
							break;
						case ' ': //empty space
							aux_cell = empty;
							break;
						case 'K': //Key
							aux_cell = empty;
							keys.insert(std::make_pair(ydim,i)); //row, column (y,x) (i,j)
							break;
						case 'S': //Snake
							aux_cell = snake;
							break;
						case 'O': //Stone
							aux_cell = empty;
							stones.push_back(std::make_pair(ydim,i));
							break;
						case 'E': //Exit cells are considered empty but its position is anotated as goal
							if(goal[0] == -1){
								aux_cell = empty;
								goal[1] = ydim; //i -- row --y
								goal[0] = i; //j -- column -- x
							}
							else throw constExc("You cannot have more than 1 door.\n");
							break;
						case 'A': //AL cell is considered empty but its position is selected as the initial one
							aux_cell = empty;
							al.first = ydim;
							al.second = i;
							break;
						default: 
							constExc("corrupted map!\n");
					}

					maze_grid.push_back(aux_cell); //Adding the new cell

				}

				++ydim; //Adding new

			} while(std::getline(inputMap, aux_line)); //Until no new line is found

			if(goal[0] == -1) throw constExc("You should have 1 door and 1 AL.\n");

			inputMap.close(); //Closing the file	

			//Creating initial state
			initial_state = std::make_unique<t_state>(keys, stones, al, 0, h_default);

		} catch(std::exception &e){
			inputMap.close(); //Closing the file
      		std::cerr << "Exception in constructor,  " << e.what() << std::endl;
      		exit(-1);
  		}

}

//returns true when the given pstd::coutition is goal
bool Maze::isGoal(int row, int col) {
	return (col == goal[0] && row == goal[1]); //col -> x row -> y
}

//overloading () operator to obtain the current cell
cell_type& Maze::operator()(int row, int col){

	//std::cout << row << " " << col << '\n';
	 
	//Ensuring safety
	assert(col >= 0 && col < xdim);
	assert(row >= 0 && row < ydim);

	//Returning the asked cell type
	return maze_grid[col + row*xdim];
}


//Printing the map for debugging 
void Maze::print(){

	//checking map
	for(int i = 0; i < ydim; ++i){ //For each row
		for(int j= 0; j < xdim; ++j){ //For each column in the row
			switch ((*this)(i,j)){
				case wall: //Non traversable
					std::cout << "%";
					break;
				case empty: //empty space
					if(isGoal(i,j)) std::cout << "E";
					else std::cout << " ";
					break;
				case snake: //Snake
					std::cout << "S";
					break;
				default: //unrecognizd tipe
					std::cout << "error";
			}
		}
		//newline
		std::cout << '\n';

	}

	//initial state
	initial_state->print();

}