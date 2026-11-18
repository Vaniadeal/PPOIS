#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include "sorts/SortUtils.hpp"

namespace lab4 {

template <typename RandomIt, typename Compare = std::less<>>
void bogo_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    if (first == last || (first + 1) == last) {
        return;
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    while (!is_sorted_custom(first, last, comp)) {
        std::shuffle(first, last, generator);
    }
}

template <typename Container, typename Compare = std::less<>>
void bogo_sort_container(Container& container, Compare comp = Compare()) {
    using std::begin;
    using std::end;
    bogo_sort(begin(container), end(container), comp);
}

} // namespace lab4
