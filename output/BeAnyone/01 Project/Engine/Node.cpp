#include "pch.h"
#include "Node.h"

void Node::setData(int data)
{
	this->data = data;
}

void Node::print()
{
	cout << "data = " << this->data << endl;
}