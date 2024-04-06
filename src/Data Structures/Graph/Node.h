/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

#ifndef NODE_H
#define NODE_H

#include <any>

class Node
{
public:
	Node() : id(-1), data() {} // Default constructor

	Node(int id, const std::any &data) : id(id), data(data){};

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
	std::any data;
};

#endif
