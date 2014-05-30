/*
 * node.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Jatin
 */

#include "node.h"

node::node() {
	is_paired=0;
	// TODO Auto-generated constructor stub

}

void node:: print_node_info(){
	std::cout<<"\nNode id : "<<node_id;
	std::cout<<"\tPosition:"<<position;
	std::cout<<"\tvelocity:"<<direction;

}
void node:: change_right_is_paired(bool value)
{
	right_node->is_paired=value;
}
node::~node() {
	// TODO Auto-generated destructor stub
}

