#include <iostream>
#include <string>
#include "maze.hpp"
#include "t_state.hpp"

int main(int argc, char ** argv){
	
	Maze map(argv[1]);
	map.print();

	//Checking move
	t_state next = (map.initial_state)->move(up);
	next.print();

 	next = (map.initial_state)->move(down);
	next.print();

	next = (map.initial_state)->move(right);
	next.print();

	next = (map.initial_state)->move(left);
	next.print();


	return 0;

}

/*vestor<t_state> get_Descend(const t_state &n) const{
	Make inside a class that contains statistics and al the stuffs

} */

/*
	sort(children);
	merge(chidrebeg,childrenend,...)
*/

/*
	To do:
	 * Finish middle todos
	 * include opoen and closed

	 * CREATE AGENT
	 * Create astar inside
	 * Main

*/