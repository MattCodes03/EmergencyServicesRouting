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

            QuickSort(queue);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in EnQueue: " << e.what() << std::endl;
        }
    };

    void DeQueue()
    {
        lock_guard<mutex> lock(*queueMutex);

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

private:
    vector<T> queue{};
    shared_ptr<mutex> queueMutex = make_shared<mutex>();
};

#endif