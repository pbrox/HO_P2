//Header file for maze class
#ifndef MAZE_CS
#define MAZE_CS

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <assert.h>    
#include <iostream>
#include <memory>


//Antipated declaration to avoid circular dependencies 
class t_state;

//enum to determine cell states
enum cell_type {empty, snake, wall};

//maze class

class Maze{

	public:
		//Constructor
		Maze(std::string path); //Reads the maze from file and creates it

		//Destructor
		~Maze();

		//returns true when the given position is goal
		bool isGoal(int row, int col) const ;

		//overloading () operator to obtain the current cell
		cell_type operator()(int row, int col) const;

		//printing the map (for debug)
		void print() const;

		//Grid dimensions
		int xdim;
		int ydim;

		//Init position of the AI
		 std::unique_ptr<t_state> initial_state;


	private:

		//Goal cell
		int goal[2]; //(x,y) -- (j,i) -- (col,row)

		//Grid itself
		std::vector<cell_type> maze_grid; //Dimensions n*m, row major
	
};


#endif