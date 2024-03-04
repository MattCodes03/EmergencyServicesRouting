#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include "../Utils.h"
#include <iostream>

using namespace std;

template <typename T>
class Queue
{
public:
    Queue(){};

    void EnQueue(const T &item)
    {
        // Push onto queue
        queue.push_back(item);
    };

    void DeQueue(const T &item)
    {
        // Check if the queue is not empty before dequeuing
        if (!queue.empty())
        {
            cout << "DeQueued: " << queue.front() << "\n";
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

private:
    vector<T> queue{};
};

#endif