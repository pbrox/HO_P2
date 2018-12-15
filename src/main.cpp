#include <iostream>
#include <string>
#include "maze.hpp"
#include "t_state.hpp"
#include "closed_list.hpp"
#include "open_list.cpp"


int main(int argc, char ** argv){
	
	Maze map(argv[1]);
	map.print();

	//Checking move
	auto  next_up = (map.initial_state)->move(up);
	next_up.print();

 	auto next_down = (map.initial_state)->move(down);
	next_down.print();

	auto next = (map.initial_state)->move(right);
	next.print();

	next = (map.initial_state)->move(left);
	next.print();


	//Ckecks for opened and closed list ESTADO A
	astar_node a = std::make_pair(*map.initial_state, std::vector<std::pair<int,int>>(0));

	//differents ESTADO B != A
	astar_node b = std::make_pair(next_up, std::vector<std::pair<int,int>>(0));

	//Igual
	auto c = b;

	closed_list close;
	std::cout << close.push(a) << ' ' << close.belongs(a) << ' ' << close.belongs(b) <<' ' << close.push(b) << ' ' << close.push(c) << '\n';
	// 1 1 0 1 0

	//Checks for the opened list
	open_list opened(1024);

	//Insert node with f = 2, f = 4, f = 2 but G = 1, f =4 but G = 1
	opened.push(std::make_pair(next_up,  std::vector<std::pair<int,int>>(0)));
	opened.push(std::make_pair(next_down, std::vector<std::pair<int,int>>(0))); //Mueve piedra!
	
	//Pop nodes until end
	while(!opened.is_empty()){
		auto curr = opened.pop();
		curr.first.print();
	}




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
	 
	 * CREATE AGENT
	 * Create astar inside
	 * Main
	 * HELP

*/