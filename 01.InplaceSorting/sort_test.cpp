#include <algorithm>
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "sorting.h"

namespace
{
std::vector<int> sorted_by_std(std::vector<int> values)
{
    std::sort(values.begin(), values.end());
    return values;
}

template <typename Sorter>
void expect_std_order(std::vector<int> values, Sorter sorter)
{
    const std::vector<int> expected = sorted_by_std(values);
    sorter(values.begin(), values.end());
    EXPECT_EQ(values, expected);
}

std::vector<int> make_data(std::size_t size, unsigned seed)
{
    std::mt19937 engine(seed);
    std::vector<int> values(size);
    for (int& value : values)
    {
        value = static_cast<int>(engine() % 200001U) - 100000;
    }
    return values;
}
} // namespace

TEST(BubbleSort, EmptyAndSingle)
{
    expect_std_order({}, [](auto begin, auto end)
    {
        bubble_sort(begin, end);
    });
    expect_std_order({42}, [](auto begin, auto end)
    {
        bubble_sort(begin, end);
    });
}

TEST(BubbleSort, CommonCases)
{
    expect_std_order({1, 2, 3, 4}, [](auto begin, auto end)
    {
        bubble_sort(begin, end);
    });
    expect_std_order({4, 3, 2, 1}, [](auto begin, auto end)
    {
        bubble_sort(begin, end);
    });
    expect_std_order({3, 1, 4, 1, 5, 9, 2, 6, 5}, [](auto begin, auto end)
    {
        bubble_sort(begin, end);
    });
}

TEST(QuickSort, EmptyAndSingle)
{
    expect_std_order({}, [](auto begin, auto end)
    {
        quick_sort(begin, end);
    });
    expect_std_order({7}, [](auto begin, auto end)
    {
        quick_sort(begin, end);
    });
}

TEST(QuickSort, CommonCases)
{
    expect_std_order({1, 2, 3, 4, 5}, [](auto begin, auto end)
    {
        quick_sort(begin, end);
    });
    expect_std_order({9, 8, 7, 6, 5}, [](auto begin, auto end)
    {
        quick_sort(begin, end);
    });
    expect_std_order({2, 2, 2, 1, 0, 1}, [](auto begin, auto end)
    {
        quick_sort(begin, end);
    });
}

TEST(SortingIntegration, GeneratedDataBubble)
{
    const std::size_t sizes[] = {0, 1, 2, 3, 10, 50, 100};
    for (std::size_t size : sizes)
    {
        expect_std_order(make_data(size, 41U + static_cast<unsigned>(size)),
                         [](auto begin, auto end)
        {
            bubble_sort(begin, end);
        });
    }
}

TEST(SortingIntegration, GeneratedDataQuick)
{
    const std::size_t sizes[] = {0, 1, 2, 3, 10, 50, 100};
    for (std::size_t size : sizes)
    {
        expect_std_order(make_data(size, 83U + static_cast<unsigned>(size)),
                         [](auto begin, auto end)
        {
            quick_sort(begin, end);
        });
    }
}
