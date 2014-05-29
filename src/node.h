/*
 * node.h
 *
 *  Created on: May 28, 2014
 *      Author: Jatin
 */

#ifndef NODE_H_
#define NODE_H_
#include <vector>
#include <iostream>
class node {
	double position ;//position of the bead. It can be 0<position<100.
	double direction ;//direction of the velocity. Either -1 or 1.
	double velocity ;// Velocity of bead
	double radius;//radius of the bead
	node * left_node;
	node * right_node;

public:
	node();
	node(double p,double d,double r,double v):position(p),direction(d),
			radius(r),velocity(v){};
	void add_left_node(node & l){left_node =&l;};
	void add_right_node(node & r){right_node= &r;};
	virtual ~node();
};

#endif /* NODE_H_ */
