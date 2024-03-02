#pragma once
#include <random>

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