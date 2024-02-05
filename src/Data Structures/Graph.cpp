#include "Graph.h"
#include <iostream>
#include <iomanip>

using namespace std;

Graph::Graph() : numOfVerticies(), graph() {}

void Graph::DisplayAsMatrix()
{
	int i, j;
	cout << "\n\n" << setw(4) << " ";

	for (i = 0; i < this->numOfVerticies; i++)
	{
		cout << setw(3) << "(" << i << ")";
	}
	cout << "\n\n";

	for (i = 0; i < this->numOfVerticies; i++)
	{
		cout << setw(3) << "(" << i << ")";
		for (j = 0; j < this->numOfVerticies; j++)
		{
			cout << setw(4) << this->graph[i][j];
		}
		cout << "\n\n";
	}
}

void Graph::AddVertex(std::string vertex)
{
	this->Verticies[this->numOfVerticies] = vertex;
	this->numOfVerticies++;
}

void Graph::AddEdge(int v1, int v2)
{
	this->graph[v1][v2] = 1;
	this->graph[v2][v1] = 1;
}

void Graph::RemoveEdge(int v1, int v2)
{
	this->graph[v1][v2] = 0;
	this->graph[v2][v1] = 0;
}
