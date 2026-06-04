#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "collvalue.h"
#include "sorting.h"

using CollInt = CollectingValue<int>;

std::vector<CollInt> generate_data(std::size_t size)
{
    std::vector<CollInt> data(size);
    std::iota(data.begin(), data.end(), CollInt(0));
    return data;
}

void shuffle_data(std::vector<CollInt>& data, std::size_t salt)
{
    std::mt19937 engine(100U + static_cast<unsigned>(salt));
    std::shuffle(data.begin(), data.end(), engine);
}

void print_stats(std::size_t size, const char *name)
{
    std::cout << size << '\t' << name << "\t\t" << CollInt::comps << "\t\t"
              << CollInt::swaps << "\t\t" << CollInt::moves << '\n';
}

int main()
{
    const std::vector<std::size_t> sizes = {0, 1, 10, 100, 500, 1000, 2000};

    std::cout << "N\tAlgo\t\tComps\t\tSwaps\t\tMoves\n";
    std::cout << "----------------------------------------------------------------\n";

    for (std::size_t size : sizes)
    {
        auto data = generate_data(size);

        shuffle_data(data, size);
        CollInt::reset_stats();
        std::sort(data.begin(), data.end());
        assert(std::is_sorted(data.begin(), data.end()));
        print_stats(size, "std::sort");

        shuffle_data(data, size + 1);
        CollInt::reset_stats();
        bubble_sort(data.begin(), data.end());
        assert(std::is_sorted(data.begin(), data.end()));
        print_stats(size, "Bubble");

        shuffle_data(data, size + 2);
        CollInt::reset_stats();
        quick_sort(data.begin(), data.end());
        assert(std::is_sorted(data.begin(), data.end()));
        print_stats(size, "Quick");

        std::cout << "--------------------------------------------------------------\n";
    }

    return 0;
}
