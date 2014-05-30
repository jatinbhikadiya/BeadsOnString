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
			if (p == j || abs(p - j) < diff || p==0) {
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

	std::cout << "\nPositions\t\t: ";
	for (auto p : positions) {
		std::cout << p << "\t";
	}
	std::cout << "\nVelocity directions\t: ";
	for (auto v : velocities) {
		std::cout << v << "\t";
	}
}

void init_nodes(std::vector<node*> &all_nodes, std::vector<int>&positions,
		std::vector<int>&velocities, double r, int N) {

	node *left_surface = new node(all_nodes.size(), 0, 1, 0, 0);

	all_nodes.push_back(left_surface);

	for (int i = 0; i < N; i++) {
		node* temp = new node(all_nodes.size(), positions.at(i),
				velocities.at(i), r, 1);
		all_nodes.push_back(temp);

		all_nodes.at(i + 1)->add_left_node(all_nodes.at(i));
		all_nodes.at(i)->add_right_node(all_nodes.at(i + 1));
	}
	node* right_surface = new node(all_nodes.size(), 100, -1, 0, 0);
	all_nodes.push_back(right_surface);
	all_nodes.at(N)->add_right_node(all_nodes.at(N + 1));
	all_nodes.at(N + 1)->add_left_node(all_nodes.at(N));

}

void print_nodes(std::vector<node*> &all_nodes) {
	int count = 0;
	for (auto i : all_nodes) {
		std::cout << "\n\ncount :" << count;
		i->print_node_info();

		/*if (count != 0) {
			std::cout << "\nLeft node is :";
			i->get_left_node()->print_node_info();
		}
		if (count != all_nodes.size() - 1) {
			std::cout << "\nRight node is :";
			i->get_right_node()->print_node_info();
		}*/
		count++;
	}
	std::cout<<std::endl;
}

void find_initial_pairs(std::vector<node*> &all_nodes,
		std::vector<std::pair<int, int> > &pairs) {
	for (auto i : all_nodes) {
		std::cout << "\nnode is: " << i->get_node_id();
		if (!i->is_paired) {
			int r_dir, i_dir = i->get_direction();
			std::cout << "\ti direction : " << i_dir;
			if (i_dir == 1) {

				r_dir = i->get_right_node()->get_direction();
				std::cout << "\tr direction : " << r_dir;

				if (r_dir == -1 && i->get_right_node()->is_paired == 0) {
					std::pair<int, int> temp = std::make_pair(i->get_node_id(),
							i->get_right_node()->get_node_id());
					std::cout << "\tpair found 1 :" << temp.first << "\t"
							<< temp.second;
					pairs.push_back(temp);
					i->get_right_node()->is_paired = 1;
					i->is_paired = 1;
				}
			}
		}
	}
	std::cout << std::endl;
}

bool sort_time(const std::pair<int, double> &a,
		const std::pair<int, double>&b) {
	return a.second < b.second;
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
void find_positions(std::vector<node*> &all_nodes, double time) {

	std::cout << "time entered is " << time << std::endl;
	while (time > 0) {
		std::cout << "\nprinting pairs\n";
		std::vector<std::pair<int, int> > pairs;
		find_initial_pairs(all_nodes, pairs);

		std::vector<std::pair<int, double>> time_to_impact;

		std::cout << "total pairs : " << pairs.size();
		int ind = 0;
		for (auto i : pairs) {
			double temp = all_nodes.at(i.first)->time_to_impact(
					all_nodes.at(i.second));
			std::cout << temp << "t";
			time_to_impact.push_back(std::make_pair(ind, temp));
			ind++;
		}
		sort(time_to_impact.begin(), time_to_impact.end(), sort_time);

		std::cout << "\ntime to impact\n";
		for (auto i : time_to_impact) {
			std::cout << "<" << i.first << "," << i.second << ">\t";
		}

		std::vector<int> pair_ids_to_collide;
		pair_ids_to_collide.push_back(time_to_impact.at(0).first);

		for (unsigned int i = 1; i < time_to_impact.size(); i++) {
			if (time_to_impact.at(0).second == time_to_impact.at(i).second) {
				pair_ids_to_collide.push_back(time_to_impact.at(i).first);
			} else {
				break;
			}
		}

		std::cout << "\nPair ids to collide\n";

		for (auto i : pair_ids_to_collide) {
			std::cout << i << "\t";
		}
		std::cout << "\n";

		double temp_time = time_to_impact.at(0).second;
		std::cout<<" Temp time :"<<temp_time<<std::endl;
		std::cout<<" Time :"<<time<<std::endl;
		if (temp_time < time) {
			for (auto i : all_nodes) {
				i->update_position_at_t(temp_time);
				i->is_paired = 0;
			}
			for(auto i:pair_ids_to_collide)
			{
				all_nodes.at(pairs.at(i).first)->change_direction();
				all_nodes.at(pairs.at(i).second)->change_direction();
			}
			print_nodes(all_nodes);
			time -= temp_time;

		} else {
			for (auto i : all_nodes) {
				i->update_position_at_t(time);
				i->is_paired = 0;
			}
			break;
		}
	}
}

void free_nodes(std::vector<node*> all_nodes) {
	for (auto i : all_nodes) {
		delete[] i;
	}
	all_nodes.clear();
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
	int temp = 1+ rand() % 100;
	//radius can be any number between 0 to 10/2*N
	double radius = double(temp) / (500);
	std::cout << "Radius is " << radius << std::endl;

	//Get position and velocity direction of the bead
	std::vector<int> positions;
	std::vector<int> velocities;

	init_positions(positions, velocities, radius, N);

	std::vector<node*> all_nodes;
	init_nodes(all_nodes, positions, velocities, radius, N);

	std::cout << "\nTotal beads are : " << N << std::endl;
	std::cout << "Total nodes including two end surfaces :" << all_nodes.size()
			<< std::endl;

//	/print_nodes(all_nodes);
	std::cout << "\nNode initialization is done...\n";

//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
//	all_nodes.at(1)->is_paired=1;
//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
//	all_nodes.at(0)->get_right_node()->is_paired=0;
//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(1)->is_paired;

	double time;
	std::cout << "Enter value of T to find out positions at time T\n";
	std::cin>>time;
	find_positions(all_nodes, time);
	print_nodes(all_nodes);
//	/free_nodes(all_nodes);
	return 0;
}
