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

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Node.h"
#include "../Priority Queue/Queue.h"
#include "../../Types/Nodes/Emergency.h"
#include <iostream>
#include <cmath>
#include <mutex>
#include <memory>

class Graph
{
public:
    Graph(int numNodes, const any &defaultData) : adjacencyList(numNodes), nodes(numNodes), queue()
    {
        for (int i = 0; i < numNodes; ++i)
        {
            nodes[i] = Node(i, defaultData);
        }
    }

    /*
    Function for adding eddge to graph
    @param destination Destinations Nodes ID
        @param weight Weight of this edge
    */
    void AddEdge(int source, int destination, int weight)
    {
        // Check if the edge already exists
        for (const auto &neighbor : adjacencyList[source])
        {
            if (neighbor.first == destination)
            {
                // Edge already exists, return without adding it again
                return;
            }
        }
        // Edge does not exist, add it to the graph
        adjacencyList[source].emplace_back(destination, weight);
        sort(adjacencyList[source].begin(), adjacencyList[source].end());
    }

    /*
Function will add node to graph
@param  node Node being added
 */

    void AddNode(const Node &node)
    {
        nodes[node.GetID()] = node;
    };

    /*
Function will remove node to graph
@param  id ID of node to be removed
 */
    void RemoveNode(int id)
    {
        nodes[id] = Node(id, 42);
    }

    // Function will clear the graph
    void ClearGraph()
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            nodes[i] = Node(i, 42);
        }
    };

    // Euclidean Distance Formula
    int CalculateDistance(pair<int, int> location1, pair<int, int> location2)
    {
        return sqrt(pow(location1.first - location2.first, 2) + pow(location1.second - location2.second, 2));
    }

    void Display()
    {
        for (int i = 0; i < adjacencyList.size(); ++i)
        {
            std::cout << "Node " << nodes[i].GetID() << " --> ";
            for (const auto &neighbor : adjacencyList[i])
            {
                std::cout << "(Node " << nodes[neighbor.first].GetID() << ", " << neighbor.second << ") ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << std::endl;
    }

    // Int 1 is Source Node (Emergency or Ambulance), Int 2 is Closest Ambulance or Hospital found by the algorithm
    std::pair<int, int> Dijkstra(int source)
    {
        int size = adjacencyList.size();
        std::vector<int> distance(size, numeric_limits<int>::max());
        std::vector<int> parent(size, -1);
        std::vector<bool> visited(size, false);

        distance[source] = 0;

        while (true)
        {
            int minDistanceIndex = -1;                    // Index of Node with the lowest distance to source
            int minDistance = numeric_limits<int>::max(); // Lowest distance value found so far, initialize to maximum value

            // Find the node with the minimum distance among unvisited nodes
            for (int i = 0; i < size; i++)
            {
                if (!visited[i] && distance[i] < minDistance)
                {
                    minDistance = distance[i];
                    minDistanceIndex = i;
                }
            }

            if (minDistanceIndex == -1) // If no unvisited nodes found
                break;

            visited[minDistanceIndex] = true; // Mark the current node as visited

            // Update distances to neighbors of minDistanceIndex
            for (const auto &neighbor : adjacencyList[minDistanceIndex])
            {
                int nodeID = neighbor.first;
                int weight = neighbor.second;
                if (!visited[nodeID] && distance[minDistanceIndex] + weight < distance[nodeID])
                {
                    distance[nodeID] = distance[minDistanceIndex] + weight;
                    parent[nodeID] = minDistanceIndex;
                }
            }
        }

        // Find the destination node
        bool destinationFound = false;
        int destination = -1;
        int minDistance = numeric_limits<int>::max(); // Initialize to maximum value
        for (int i = 0; i < size; ++i)
        {
            if (i != source && distance[i] < minDistance && distance[i] != numeric_limits<int>::max())
            {
                destination = i;
                minDistance = distance[i]; // Update minDistance
                destinationFound = true;
            }
        }

        // If no destination found, set it to -1
        if (!destinationFound)
        {
            destination = -1;
        }

        return {source, destination};
    }

    std::vector<Node> GetNodes() const
    {

        return this->nodes;
    }

    // Mutex is used to ensure thread safety during emergency routing process
    std::mutex &GetMutex()
    {
        return *graphMutex;
    }

    vector<vector<pair<int, int>>> GetAdjacencyList() const { return this->adjacencyList; };

    Queue<Emergency> queue;

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;
    vector<Node> nodes;

    // Mutex used for Thread Safety
    std::shared_ptr<std::mutex> graphMutex = std::make_shared<std::mutex>();
};

#endif