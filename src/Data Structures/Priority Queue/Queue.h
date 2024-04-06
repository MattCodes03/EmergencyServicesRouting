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

#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include "../Utils.h"
#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class Queue
{
public:
    Queue(){};

    void EnQueue(const T &item)
    {
        try
        {
            // Push onto queue
            queue.push_back(item);

            // Maintain heap property by heapifying up or down depending on heap type
            int index = queue.size() - 1; // Index of the newly inserted element
            while (index > 0)
            {
                int parentIndex = (index - 1) / 2; // Calculate parent index
                if (compareItems(queue[index], queue[parentIndex]))
                {
                    swap(queue[index], queue[parentIndex]); // Swap child with parent
                    index = parentIndex;                    // Update index to parent
                }
                else
                {
                    break; // Heap property satisfied
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in EnQueue: " << e.what() << std::endl;
        }
    };

    void DeQueue()
    {

        // Check if the queue is not empty before dequeuing
        if (!queue.empty())
        {
            queue.erase(queue.begin()); // Remove the front element
        }
        else
        {
            std::cout << "Queue is empty! Cannot DeQueue.\n";
        }
    };

    void Display()
    {
        if (!queue.empty())
        {
            for (const T &item : queue)
            {
                std::cout << item << "\n";
            }
        }
        else
        {
            std::cout << "No items in Queue!\n";
        }
    }

    std::vector<T> &GetQueue()
    {
        return queue;
    }

    // Mutex is used to ensure thread safety during emergency routing process
    std::mutex &GetMutex()
    {
        return *queueMutex;
    }

    void SetQueueType(string type)
    {
        this->type = type;
    }

private:
    std::vector<T> queue{};

    // Mutex for Thread Safety
    std::shared_ptr<std::mutex> queueMutex = std::make_shared<std::mutex>();

    // Queue type
    std::string type = "MIN";

    // Comparison function for items of type T
    bool compareItems(const T &a, const T &b)
    {
        if (type == "MAX")
        {
            return a > b;
        }
        else
        {
            return a < b;
        }
    }
};

#endif
