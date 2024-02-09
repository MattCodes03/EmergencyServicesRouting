#pragma once
#include <any>

using namespace std;

class Node
{
public:
	Node(int id, int weight) : id(id), weight(weight){};

	void SetData(any &data)
	{
		this->Data = data;
	};

	int GetWeight()
	{
		return this->weight;
	};

private:
	any Data;
	int weight;
	int id;
};
