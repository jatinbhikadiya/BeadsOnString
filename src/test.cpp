//============================================================================
// Name        : test.cpp
// Author      : Jatin Bhikadiya
// Version     : 1.0
// Description : Beads on string for the VideoIQ interview
/* Here for each bead there is one node.
 * Also there are two dummy nodes for each surface.
 * So there will be total N+2 nodes.
 *
 * Each node contains the position of the bead, direction
 * of the velocity and two neighbors: left and right.
 * For the first bead left neighbor is node 0(left surface)
 * and for the last bead right neighbot is node N+1(right surface)
 *
 *
 * After initialization is done, nodes pairs which are going to collide  in
 * some time 't' are collected. Amongst the collected node pairs, the node pair
 * which is going to collide first in time "temp" is selected.
 *
 * Now all node's position is updated to time "temp". The velocity direction of the
 * nodes in colliding pair will be changed. No other node will change it's
 * direction before time "temp", so no need to change direction of other nodes.
 *
 * Above process is repeated till the system reaches to time entered by user.
 *
 */
//============================================================================
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <iomanip>
#include<cmath>
#include "node.h"

/*
 * This function initializes the positions and velocity direction of the all beads.
 * It selects the value of bead position randomly between 0 to 100.
 * FOr simplicity initial position values are taken as integer values.
 * But once all nodes(beads) are initialized position value can be any
 * double value between 0 to 100 based on the velocity.
 * The velocity direction is randomly selected as 1 or -1.
 * 1 indicated that bead is travelling in in the direction of surface at 0
 * to surface at 100. -1 indicated other way.
 */
void init_positions(std::vector<int>&positions, std::vector<int>&velocities,
		double radius, int N) {
	double diff = (2 * radius);
	for (int i = 0; i < N; i++) {
		int p = radius + rand() % 99;
		int v = rand() % 2;
		bool exist = 0;
		for (auto j : positions) {
			if (p == j || abs(p - j) < diff || p == 0) {
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

}

/*
 * This function initializes all the nodes. There is one node for each
 * bead. Also there is associated node with both end surfaces.
 * So given N beads, there will be total N+2 nodes initialized.
 * Nodes are initialized with randomly generated position and
 * velocity directions. Radius is also randomly selected and it is
 * same for all. Velocity magnitude is constant and is taken 1 for simplicity.
 * Two special case:
 * Left surface : for left surface position in 0, velocity direction is
 * 1(left to right is 1), radius 0 and velocity magnitude 0.
 * Right surface : for right surface position in 100, velocity direction is
 * -1(left to right is -1), radius 0 and velocity magnitude 0.
 */

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

/*
 * This function prints all the nodes given in argument
 */
void print_nodes(std::vector<node*> &all_nodes) {
	std::cout
			<< "\n-----------------Printing node position and velocity direction------------------\n";
	std::cout << std::left << std::setw(12) << "Node ids" << ":" << std::right;
	for (auto i : all_nodes) {
		int id = i->get_node_id();
		if (id == 0) {
			std::cout << std::setw(8) << "L end";
		} else if (id == all_nodes.size() - 1) {
			std::cout << std::setw(8) << "R end";
		} else {
			std::cout << std::setw(8) << id;
		}
	}
	std::cout << std::endl;

	std::cout << std::left << std::setw(12) << "Positions" << ":" << std::right;
	for (auto i : all_nodes) {
		std::cout << std::setw(8) << std::setprecision(5) << i->get_position();
	}
	std::cout << std::endl;

	std::cout << std::left << std::setw(12) << "V Directions" << ":"
			<< std::right;
	for (auto i : all_nodes) {
		std::cout << std::setw(8) << i->get_direction();
	}
	std::cout << std::endl;
}

/*
 * This function find pairs of nodes. Each pair consist of
 * two neighbor nodes those are travelling in the opposite direction.
 * There are two possibilities when they have opposite direction
 * 1: They are coming near to each other
 * 2: They are moving away to each other
 * In our case, nodes which are coming near each other are
 * selected.
 */
void find_initial_pairs(std::vector<node*> &all_nodes,
		std::vector<std::pair<int, int> > &pairs) {
	for (auto i : all_nodes) {
		if (!i->is_paired) {
			int r_dir, i_dir = i->get_direction();
			if (i_dir == 1) {

				r_dir = i->get_right_node()->get_direction();

				if (r_dir == -1 && i->get_right_node()->is_paired == 0) {
					std::pair<int, int> temp = std::make_pair(i->get_node_id(),
							i->get_right_node()->get_node_id());
					pairs.push_back(temp);
					i->get_right_node()->is_paired = 1;
					i->is_paired = 1;
				}
			}
		}
	}
}

/* Helper function for index sort
 *
 */
bool sort_time(const std::pair<int, double> &a,
		const std::pair<int, double>&b) {
	return a.second < b.second;
}

/* To find the position at time T
 * First, node pairs are found in which two nodes
 * are going to collide in some time t. Amongst these
 * pairs, the pair with minimum collision time is found and
 * the state of the system is changes at that minimum time.
 * So adding all these minimum time until the system reaches
 * to total time T will give the position of all the beads after
 * time T
 */
void find_positions(std::vector<node*> &all_nodes, double time) {

	std::cout << "Time entered is " << time << std::endl;
	double global_time = 0;
	int iterations=0;
	std::cout << "-----Initial collision----- " << std::endl;
	while (time > 0) {
		iterations++;
		std::vector<std::pair<int, int> > pairs;
		find_initial_pairs(all_nodes, pairs);

		std::vector<std::pair<int, double>> time_to_impact;
		std::cout << "Total node pairs which are going to collide : "
				<< pairs.size() << std::endl;
		int ind = 0;
		for (auto i : pairs) {
			double temp = all_nodes.at(i.first)->time_to_impact(
					all_nodes.at(i.second));
			time_to_impact.push_back(std::make_pair(ind, temp));
			ind++;
		}
		sort(time_to_impact.begin(), time_to_impact.end(), sort_time);

		std::cout
				<< "Pairs sorted in increasing order of time to impact : <pair, pair_index, time to impact>\n";
		int index = 0;
		for (auto i : time_to_impact) {
			std::cout << "<(" << pairs.at(index).first << ","
					<< pairs.at(index).second << ")";
			std::cout << ", " << i.first << ", " << i.second << ">\t";
			index++;
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

		std::cout
				<< "\nPair ids to collide(More than one pair can collide at the same time) : ";

		for (auto i : pair_ids_to_collide) {
			std::cout << i << "\t";
		}
		std::cout << "\n";

		double temp_time = time_to_impact.at(0).second;
		std::cout << "Time for impact : " << temp_time << std::endl;
		std::cout << "Total remaining time: " << time << std::endl;
		if (temp_time < time) {
			global_time += temp_time;
			for (auto i : all_nodes) {
				i->update_position_at_t(temp_time);
				i->is_paired = 0;
			}
			for (auto i : pair_ids_to_collide) {
				all_nodes.at(pairs.at(i).first)->change_direction();
				all_nodes.at(pairs.at(i).second)->change_direction();
			}
			//comment below 2 line for less verbose
			std::cout << "\nNodes position after time : " << global_time;
			print_nodes(all_nodes);

			time -= temp_time;

		} else {
			for (auto i : all_nodes) {
				i->update_position_at_t(time);
				i->is_paired = 0;
			}
			global_time += time;
			std::cout << "\nFinal beads position after time : " << global_time;
			print_nodes(all_nodes);
			std::cout<<"Number of iterations to reach given time : "<<iterations<<std::endl;
			break;
		}
		std::cout << "\n-----Next collision----- " << std::endl;

	}
}

/*
 * This function will free memory of all the nodes.
 */

void free_nodes(std::vector<node*> all_nodes) {
	for (auto i : all_nodes) {
		delete i;
	}
	all_nodes.clear();
}

int main() {

	// Number of nodes can be between 0 to 20
	std::srand(std::time(0));
	int N = 1 + rand() % 20;
	std::cout << "Number of beads : " << N << std::endl;
	std::srand(std::time(0));
	int temp = 1 + rand() % 100;
	//radius can be any number between 0 to 10/2*N
	double radius = double(temp) / (500);
	std::cout << "Radius : " << radius << std::endl;
	std::cout << "Velocity magnitude is : " << 1;
	//Get position and velocity direction of the bead
	std::vector<int> positions;
	std::vector<int> velocities;

	init_positions(positions, velocities, radius, N);

	std::vector<node*> all_nodes;
	std::cout << "\nInitializing nodes...\n";
	init_nodes(all_nodes, positions, velocities, radius, N);
	std::cout << "\nTotal nodes including two end surfaces :"
			<< all_nodes.size() << std::endl;

//	/print_nodes(all_nodes);
	std::cout << "\nNode initialization is done...\n";
	std::cout << "\n--Initial node information-- \n";
	print_nodes(all_nodes);
//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
//	all_nodes.at(1)->is_paired=1;
//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(0)->get_right_node()->is_paired;
//	all_nodes.at(0)->get_right_node()->is_paired=0;
//	std::cout<<"\nNode 0 right node is_paired values :"<<all_nodes.at(1)->is_paired;

	double time;
	std::cout << "\nEnter value of T to find out positions at time T\n";
	std::cin >> time;
	find_positions(all_nodes, time);
	free_nodes(all_nodes);
	return 0;
}
