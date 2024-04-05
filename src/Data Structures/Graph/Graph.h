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

using namespace std;

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
    Function will add edge to the graph.
    @param  source  Source Nodes ID
    @param  destination Destinations Nodes ID
    @param  weight  Weight of this edge
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

    void AddNode(const Node &node)
    {
        nodes[node.GetID()] = node;
    };

    void RemoveNode(int id)
    {
        nodes[id] = Node(id, 42);
    }

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
    pair<int, int> Dijkstra(int source)
    {
        int size = adjacencyList.size();
        vector<int> distance(size, numeric_limits<int>::max());
        vector<int> parent(size, -1);
        vector<bool> visited(size, false);

        distance[source] = 0;

        // Use the custom priority queue
        Queue<pair<int, int>> priorityQueue;
        priorityQueue.SetQueueType("MIN"); // Set the queue type to min-heap

        // Enqueue the source node
        priorityQueue.EnQueue({0, source}); // {distance, node}

        while (!priorityQueue.GetQueue().empty())
        {
            // Extract the node with the minimum distance
            auto minDistanceNode = priorityQueue.GetQueue().front();
            priorityQueue.DeQueue();
            int minDistance = minDistanceNode.first;
            int minDistanceIndex = minDistanceNode.second;

            // If the node is already visited, continue to the next node
            if (visited[minDistanceIndex])
                continue;

            visited[minDistanceIndex] = true; // Mark the current node as visited

            // Update distances to neighbors of minDistanceIndex
            for (const auto &neighbor : adjacencyList[minDistanceIndex])
            {
                int nodeID = neighbor.first;
                int weight = neighbor.second;
                if (!visited[nodeID] && minDistance + weight < distance[nodeID])
                {
                    distance[nodeID] = minDistance + weight;
                    parent[nodeID] = minDistanceIndex;
                    priorityQueue.EnQueue({distance[nodeID], nodeID}); // Enqueue the updated distance
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

    vector<Node> GetNodes() const
    {

        return this->nodes;
    };
    vector<vector<pair<int, int>>> GetAdjacencyList() const
    {

        return this->adjacencyList;
    };

    mutex &GetMutex()
    {
        return *graphMutex;
    }

    Queue<Emergency> queue;

private:
    vector<vector<pair<int, int>>> adjacencyList;
    vector<Node> nodes;

    shared_ptr<mutex> graphMutex = make_shared<mutex>();
};

#endif