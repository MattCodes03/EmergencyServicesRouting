#ifndef NODE_H
#define NODE_H

#include <any>

using namespace std;

class Node
{
public:
	Node() : id(-1), data() {} // Default constructor

	Node(int id, const any &data) : id(id), data(data){};

	int GetID() const
	{
		return this->id;
	}

	const std::any &GetData() const
	{
		return this->data;
	}

private:
	int id;
	any data;
};

#endif
