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
    // Randomly choose pivot index within the range [low, high]
    size_t pivotIndex = low + rand() % (high - low + 1);

    // Swap pivot element with the last element
    swap(items[pivotIndex], items[high]);

    T pivot = items[high];
    size_t i = low - 1;

    // Partition around the pivot
    for (size_t j = low; j < high; ++j)
    {
        if (items[j] < pivot)
        {
            ++i;
            swap(items[i], items[j]);
        }
    }

    swap(items[i + 1], items[high]);

    return i + 1; // Return the index of the pivot
}

template <typename T>
void QuickSortRecursive(vector<T> &items, size_t low, size_t high)
{
    if (low < high)
    {
        size_t pivotIndex = Pivot(items, low, high);

        if (pivotIndex > 0)
        {
            QuickSortRecursive(items, low, pivotIndex - 1);
        }
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