#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>
#include "sorts/SortUtils.hpp"

namespace lab4 {

template <typename RandomIt, typename Compare>
void in_place_merge_custom(RandomIt first, RandomIt middle, RandomIt last, Compare comp) {
    RandomIt left = first;
    RandomIt right = middle;

    if (left == right || right == last) {
        return;
    }

    while (left < right && right < last) {
        if (!comp(*right, *left)) {
            ++left;
        } else {
            auto valueToInsert = std::move(*right);
            RandomIt shiftIndex = right;

            while (shiftIndex != left) {
                *shiftIndex = std::move(*(shiftIndex - 1));
                --shiftIndex;
            }

            *left = std::move(valueToInsert);

            ++left;
            ++right;
        }
    }
}

template <typename RandomIt, typename Compare = std::less<>>
void in_place_merge_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    auto count = std::distance(first, last);
    if (count <= 1) {
        return;
    }

    RandomIt middle = first;
    std::advance(middle, count / 2);

    in_place_merge_sort(first, middle, comp);
    in_place_merge_sort(middle, last, comp);

    in_place_merge_custom(first, middle, last, comp);
}

template <typename Container, typename Compare = std::less<>>
void in_place_merge_sort_container(Container& container, Compare comp = Compare()) {
    using std::begin;
    using std::end;
    in_place_merge_sort(begin(container), end(container), comp);
}

} // namespace lab4
