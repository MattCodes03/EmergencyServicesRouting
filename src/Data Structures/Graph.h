#include <iostream>

#pragma once
class Graph
{
public:
	// Fields
	int numOfVerticies = 0;
	std::string Verticies[50];
	int graph[50][50];

	/*
	* Constructor
	*/
	Graph();

	// Methods


	/*
	* Display the Graph as an Adjacency Matrix.
	*/
	void DisplayAsMatrix();

	/*
	* Add an extra Vertex to the existing graph
	* @Param Vertex - This is the new vertex being added to the graph, this value will be stored in Verticies array.
	*/
	void AddVertex(std::string vertex);

	/*
	* Add an edge between two verticies.
	* @param Vertex 1
	* @param Vertex 2
	*/
	void AddEdge(int v1, int v2);

	/*
	* Remove an edge between two verticies.
	* @param Vertex 1
	* @param Vertex 2
	*/
	void RemoveEdge(int v1, int v2);
};

