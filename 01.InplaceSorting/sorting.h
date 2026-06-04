#ifndef SORTING_H
#define SORTING_H

#include <algorithm>
#include <iterator>

#include "collvalue.h"

namespace sort_helpers
{
template <typename RandomIt>
RandomIt pick_pivot(RandomIt first, RandomIt middle, RandomIt last)
{
    if (*first < *middle)
    {
        if (*middle < *last)
        {
            return middle;
        }
        if (*first < *last)
        {
            return last;
        }
        return first;
    }
    if (*first < *last)
    {
        return first;
    }
    if (*middle < *last)
    {
        return last;
    }
    return middle;
}

template <typename RandomIt>
RandomIt partition(RandomIt begin, RandomIt end)
{
    RandomIt pivot_place = std::prev(end);
    const auto length = std::distance(begin, end);
    RandomIt middle = begin + (length - 1) / 2;
    RandomIt pivot = pick_pivot(begin, middle, pivot_place);
    std::iter_swap(pivot, pivot_place);

    RandomIt less_end = begin;
    for (RandomIt current = begin; current != pivot_place; ++current)
    {
        if (*current < *pivot_place)
        {
            std::iter_swap(current, less_end);
            ++less_end;
        }
    }

    std::iter_swap(less_end, pivot_place);
    return less_end;
}

template <typename RandomIt>
void quick_sort_range(RandomIt begin, RandomIt end)
{
    using Diff = typename std::iterator_traits<RandomIt>::difference_type;

    for (;;)
    {
        const Diff length = std::distance(begin, end);
        if (length < 2)
        {
            return;
        }
        if (length == 2)
        {
            RandomIt second = std::next(begin);
            if (*second < *begin)
            {
                std::iter_swap(begin, second);
            }
            return;
        }

        RandomIt pivot = partition(begin, end);
        const Diff left_size = std::distance(begin, pivot);
        const Diff right_size = std::distance(std::next(pivot), end);
        if (left_size < right_size)
        {
            quick_sort_range(begin, pivot);
            begin = std::next(pivot);
        }
        else
        {
            quick_sort_range(std::next(pivot), end);
            end = pivot;
        }
    }
}
} // namespace sort_helpers

template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end)
    {
        return;
    }

    for (Iterator pass = begin; pass != end; ++pass)
    {
        bool changed = false;
        for (Iterator current = begin;; ++current)
        {
            Iterator next = std::next(current);
            if (next == end)
            {
                break;
            }
            if (*next < *current)
            {
                std::iter_swap(current, next);
                changed = true;
            }
        }
        if (!changed)
        {
            break;
        }
    }
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    sort_helpers::quick_sort_range(begin, end);
}

#endif // SORTING_H
