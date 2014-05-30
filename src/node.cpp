/*
 * node.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Jatin
 */

#include "node.h"

node::node() {
	is_paired = 0;
	// TODO Auto-generated constructor stub

}

void node::print_node_info() {
	std::cout << "\nNode id : " << node_id;
	std::cout << "\tPosition:" << position;
	std::cout << "\tvelocity:" << direction;

}

double node::time_to_impact(node* o) {
	//std::cout<<"\ncalculation of  time to impact \n";
	//std::cout<<"Position of 0: "<<this->position<<"\t position of 1: "<< o->get_position()<<std::endl;
	double distance = abs(this->position - o->get_position())
			- (this->radius + o->get_radius());
	double total_velocity = this->velocity + o->get_velocity();
	return distance / total_velocity;

}

void node::update_position_at_t(double time) {
	position = position + velocity * direction * time;
}

void node::change_direction() {
	if (velocity != 0) {
		if (direction == 1) {
			direction = -1;
		} else {
			direction = 1;
		}
	}
}

node::~node() {
	left_node = NULL;
	right_node= NULL;
	// TODO Auto-generated destructor stub
}

