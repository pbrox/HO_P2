//t_state class definition 
#ifndef STATE
#define STATE

#include <utility>
#include <vector>
#include <set>
#include <optional>
//Compile with --std=c++17 or higher
#include <algorithm>

/*

	TODO:
*/

enum moves{up, down, left, right};
enum heuristic_funcs{h_default};

//Antipated declaration to avoid circular dependencies 
class Maze;

class t_state{

	public:

		//Constructor for the initial state
		t_state( std::set<std::pair<int,int>> keys, std::vector<std::pair<int,int>> stones, std::pair<int,int> al,  int cost, heuristic_funcs heuristic_choosen) :
		keys(keys), stones(stones), AL_position(al), g(cost), used_heuristic(heuristic_choosen) {heuristic_v = heuristic(used_heuristic);}
		
		~t_state();

		void get_position(int &x, int &y); //returns the AL position
		bool can_move(moves move, const Maze & map); //returns true if for the given state and the maze the movement move is executable

				/*			 WARNING! 
					This move method assume
					that the movement is correct, un-
					defined behaviour if the movement
					is not allowed! This can be chec-
					ked beforehand with can_move.
				*/

		t_state move(moves move); //returns the state generated executing operation move
		
		bool is_final(const Maze & map); // returns true if the state is final

		//overloading of comparison between states
		bool operator== (const t_state &rhs) const;

		//overloading < operator
		bool operator< (const t_state &rhs) const;

		//printing the state, for debug purposes
		void print();

		//Getting F
		int get_f() const {return g + heuristic_v;} 

		//Getting G
		int get_g() const {return g;}


	private:
		std::set<std::pair<int,int>> keys; //Set containing all the remaining keys
		std::vector<std::pair<int,int>> stones; //Vector containing all stones
		std::pair<int,int> AL_position; // row, column, ie (y,x)
		int g;
		int heuristic_v;
		heuristic_funcs used_heuristic;

		//Heuristic functions collection
		int heuristic(heuristic_funcs choosen = h_default); //Heuristic function
		int default_h();

		//Function returning next position (no checks)
		std::pair<int,int> next_pos(moves move, std::optional<std::pair<int,int>> from);



};

//For actual use in A*, pair state, path list 
using astar_node = std::pair<t_state,std::vector<std::pair<int,int>>>;

#endif
