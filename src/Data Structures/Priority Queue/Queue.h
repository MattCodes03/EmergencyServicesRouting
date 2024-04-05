#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include "../Utils.h"
#include <iostream>
#include <memory>
#include <mutex>

using namespace std;

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

            // Maintain max-heap property by heapifying up
            int index = queue.size() - 1; // Index of the newly inserted element
            while (index > 0)
            {
                int parentIndex = (index - 1) / 2; // Calculate parent index
                if (compareItems(queue[index], queue[parentIndex]))
                {                                           // If child is greater than parent
                    swap(queue[index], queue[parentIndex]); // Swap child with parent
                    index = parentIndex;                    // Update index to parent
                }
                else
                {
                    break; // Max-heap property satisfied
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
            cout << "Queue is empty! Cannot DeQueue.\n";
        }
    };

    void Display()
    {
        if (!queue.empty())
        {
            for (const T &item : queue)
            {
                cout << item << "\n";
            }
        }
        else
        {
            cout << "No items in Queue!\n";
        }
    }

    vector<T> &GetQueue()
    {
        return queue;
    }

    // Mutex is used to ensure thread safety during emergency routing process
    mutex &GetMutex()
    {
        return *queueMutex;
    }

    void SetQueueType(string type)
    {
        this->type = type;
    }

private:
    vector<T> queue{};
    shared_ptr<mutex> queueMutex = make_shared<mutex>();

    // Queue type
    string type = "MIN";

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
