#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include "../Utils.h"

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

        // Sort the queue by priority using the QuickSort Algorithm
        if (queue.size() > 1)
        {
            QuickSort(queue);
        }
    };

    void DeQueue(const T &item){

    };

private:
    vector<T> queue;
};

#endif