#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
T GetRandom(const vector<T> &items)
{
    if (items.empty())
    {
        throw out_of_range("Vector has no data!");
    }

    // Seed random number generator
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<size_t> distribution(0, items.size() - 1);

    // Generate a random index
    size_t randomIndex = distribution(gen);

    // Return the random Emergency object from the vector
    return items[randomIndex];
};

template <typename T>
size_t Pivot(vector<T> &items, size_t low, size_t high)
{
    T pivot = items[high]; // Set Pivot value to last element in items
    size_t i = low - 1;

    // Loop through items and sort items around the pivot value
    for (int j = low; j < high; j++)
    {
        if (items[j] < pivot)
        {
            i++;
            swap(items[i], items[j]);
        }
    }

    swap(items[i + 1], items[high]);

    // Return the Pivot Index once its in its sorted position
    return i + 1;
};

template <typename T>
void QuickSortRecursive(vector<T> &items, size_t low, size_t high)
{
    if (low < high)
    {
        size_t pivotIndex = Pivot(items, low, high);

        QuickSortRecursive(items, low, pivotIndex);
        QuickSortRecursive(items, pivotIndex + 1, high);
    }
}

template <typename T>
vector<T> QuickSort(vector<T> &items)
{
    QuickSortRecursive(items, 0, items.size() - 1);
    return items;
}

#endif