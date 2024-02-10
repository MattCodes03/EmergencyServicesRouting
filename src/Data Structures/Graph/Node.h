#pragma once
#include <any>

using namespace std;

class Node
{
public:
	Node() : id(-1), data() {} // Default constructor

	Node(int id, const any &data) : id(id), data(data){};

	int GetID()
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
