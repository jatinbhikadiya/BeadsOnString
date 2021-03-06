/*
 * node.h
 *
 *  Created on: May 28, 2014
 *      Author: Jatin
 *
 * This is node class to represent bead.
 * Each bead has position, direction, radius and velocity.
 * Also each bead has knowledge(address) of its right and
 * left node.
 */

#ifndef NODE_H_
#define NODE_H_
#include <vector>
#include <iostream>
class node {
	int node_id;
	double position; //position of the bead. It can be 0<position<100.
	int direction; //direction of the velocity. Either -1 or 1.
	double radius; //radius of the bead
	double velocity; // Velocity of bead
	node * left_node;
	node * right_node;

public:
	bool is_paired;
	node();
	node(int i, double p, double d, double r, double v) :
			node_id(i), position(p), direction(d), radius(r), velocity(v) {
	}
	;
	void add_left_node(node* l) {
		left_node = l;
	}
	;
	void add_right_node(node* r) {
		right_node = r;
	}
	;
	node* get_left_node() {
		return left_node;
	}
	node* get_right_node() {
		return right_node;
	}

	int get_node_id() {
		return node_id;
	}
	int get_direction() {
		return direction;
	}
	double get_radius() {
		return radius;
	}
	double get_velocity() {
		return velocity;
	}
	double get_position() {
		return position;
	}

	double time_to_impact(node*);
	void update_position_at_t(double time);
	void change_direction();
	void print_node_info();
	virtual ~node();
};

#endif /* NODE_H_ */
