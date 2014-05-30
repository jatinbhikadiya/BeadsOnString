//============================================================================
// Name        : test.cpp
// Author      : Jatin Bhikadiya
// Version     : 1.0
// Description : Beads on string for the VideoIQ interview
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <utility>
#include<cmath>
#include "node.h"

void init_positions(std::vector<int>&positions, std::vector<int>&velocities,
		double radius, int N) {
	double diff = (2 * radius);
	for (int i = 0; i < N; i++) {
		int p = radius + rand() % 99;
		int v = rand() % 2;
		bool exist = 0;
		for (auto j : positions) {
			if (p == j || abs(p - j) < diff) {
				exist = 1;
				continue;
			} //position already exist
		}
		if (!exist) {
			positions.push_back(p);
			velocities.push_back(int(v * 2 - 1));
		} else {
			i--;
		}
	}
	std::sort(positions.begin(), positions.end());
	for (auto p : positions) {
		std::cout << p << "\t";
	}
	std::cout << "velocities\n";
	for (auto v : velocities) {
		std::cout << v << "\t";
	}
}

void init_nodes(std::vector<node*> &all_nodes, std::vector<int>&positions,
		std::vector<int>&velocities, double r, int N) {

	node *left_surface = new node(all_nodes.size(),0, 1, 0, 0);

	all_nodes.push_back(left_surface);

	for (int i = 0; i < N; i++) {
		node* temp =  new node(all_nodes.size(),positions.at(i), velocities.at(i), 1, r);
		all_nodes.push_back(temp);

		all_nodes.at(i + 1)->add_left_node(all_nodes.at(i));
		all_nodes.at(i)->add_right_node(all_nodes.at(i + 1));
	}
	node* right_surface = new node(all_nodes.size(),100, -1, 1, r);
	all_nodes.push_back(right_surface);
	all_nodes.at(N)->add_right_node(all_nodes.at(N + 1));
	all_nodes.at(N + 1)->add_left_node(all_nodes.at(N));

}

void print_nodes(std::vector<node> &all_nodes)
{
	int count=0;
	for(auto i:all_nodes)
	{
		std::cout<<"\n\ncount :"<<count;
		i.print_node_info();

		if(count!=0){
		std::cout<<"\nLeft node is :";
		i.get_left_node()->print_node_info();
		}
		if(count!=all_nodes.size()-1){
		std::cout<<"\nRight node is :";
		i.get_right_node()->print_node_info();
		}
		count++;
	}
}


/*To find the position at time T
 * First, node pairs are found in which two nodes
 * are going to collide in some time t. Amongst these
 * pairs, the pair with minimum collision time is found and
 * the state of the system is changes at that minimum time.
 * So adding all these minimum time until the system reaches
 * to total time T will give the position of all the beads after
 * time T
 */
void find_positions(std::vector<node*> &all_nodes,double time)
{

	std::cout<<"time entered is "<<time<<std::endl;
	std::vector<std::pair<int,int> > pairs;
	for( auto i : all_nodes)
	{
		std::cout<<"\nnode is is: "<<i->get_node_id();
		if(!i->is_paired)
		{
			int r_dir,i_dir = i->get_direction();
			std::cout<<"\ni direction :"<<i_dir;
			if(i_dir==1){

				r_dir =i->get_right_node()->get_direction();
				std::cout<<" r direction :"<<r_dir;

				if(r_dir==-1 && i->get_right_node()->is_paired==0)
				{
					std::pair<int,int>temp = std::make_pair(i->get_node_id(),
							i->get_right_node()->get_node_id());
					std::cout<<"pair found 1 :"<<temp.first<<"\t"<<temp.second;
					pairs.push_back(temp);
					all_nodes.at(i->get_node_id()+1)->is_paired=1;
					i->is_paired=1;
				}
			}
			/*if(i_dir==-1){
				 r_dir =i.get_right_node()->get_direction();
				std::cout<<" r direction :"<<r_dir;
				if(i.get_right_node()->get_direction()==1 && i.get_right_node()->is_paired==0)
				{
					std::pair<int,int>temp = std::make_pair(i.get_node_id(),
												i.get_right_node()->get_node_id());
					std::cout<<"pair found -1 :"<<temp.first<<"\t"<<temp.second;
					pairs.push_back(temp);
					all_nodes.at(i.get_node_id()+1).is_paired=1;
					i.is_paired=1;

				}
			}*/
		}
	}
	std::cout<<"\nprinting pairs\n";
	for(auto i:pairs){
		std::cout<<i.first<<"\t"<<i.second<<"\n";
	}
}


/* Here for each bead there is one node.
 * Also there are two dummy nodes for each surface.
 * So there will be total N+2 nodes.
 *
 * Each node contains the position of the bead, direction
 * of the velocity and two neighbors: left and right.
 * For the first bead left neighbor is node 0(left surface)
 * and for the last bead right neighbot is node N+1(right surface)
 */
int main() {

	// Number of node can be between 0 to 20
	std::srand(std::time(0));
	int N = 1 + rand() % 20;
	std::cout << "N is :" << N << std::endl;
	std::srand(std::time(0));
	int temp = rand() % 100;
	//radius can be any number between 0 to 10/2*N
	double radius = double(temp) / (500);
	std::cout << " Radius is " << radius << std::endl;

	//Get position and velocity direction of the bead
	std::vector<int> positions;
	std::vector<int> velocities;

	init_positions(positions, velocities, radius, N);

	std::vector<node*> all_nodes;
	init_nodes(all_nodes, positions, velocities, radius, N);

	std::cout << "\nBeads are : " << N << std::endl;
	std::cout << "Total nodes including two end surfaces :" << all_nodes.size()<< std::endl;

	//print_nodes(all_nodes);
	std::cout<<"Node initialization is done :\n";

	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
	all_nodes.at(1)->is_paired=1;
	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
	all_nodes.at(0)->get_right_node()->is_paired=0;
	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(1)->is_paired;

	double time;
	std::cout<<"Enter value of T to find out positions at time T\n ";
	std::cin>>time;
	find_positions(all_nodes,time);

	return 0;
}
