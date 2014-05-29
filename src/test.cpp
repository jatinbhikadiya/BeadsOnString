//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include<cmath>
using namespace std;
#include "node.h"

int main() {
	/* Here for each bead there is one node.
	 * Also there are two dummy nodes for each surface.
	 * So there will be total N+2 nodes.
	 *
	 * Each node contains the position of the bead, direction
	 * of the velocity and two neighbors: left and right.
	 * For the first bead left neighbor is node 0(left surface)
	 * and for the last bead right neighbot is node N+1(right surface)
	 */
	vector<node> all_nodes;
	// Number of node can be between 0 to 20
	std::srand(std::time(0));
	int N = 1 + rand()%20;
	cout<<"N is :"<<N<<std::endl;
	std::srand(std::time(0));
	int temp =rand()%100;
	cout<<"temp is "<<temp;
	//radius can be any number between 0 to 10/2*N
	double radius = double(temp)/ (500);
	cout<<" Radius is "<<radius<<std::endl;

	//Get position and velocity direction of the bead
	std::vector<int>positions;
	double diff = (2*radius);
	for(int i=0;i<N;i++)
	{
		int p= radius + rand()%99;
		bool exist = 0;
		for(auto j : positions)
		{
			if (p==j || abs(p-j)<diff){exist=1;continue;} //position already exist
		}
		if(!exist)
		{
			positions.push_back(p);
		}
		else{
			i--;
		}
	}
	std::sort (positions.begin(), positions.end());
	for (auto p:positions)
	{
		std::cout<<p<<"\n";
	}



	return 0;
}
