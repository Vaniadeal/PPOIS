#pragma once

#include <functional>

namespace lab4 {

template <typename ForwardIt, typename Compare = std::less<>>
bool is_sorted_custom(ForwardIt first, ForwardIt last, Compare comp = Compare()) {
    if (first == last) {
        return true;
    }

    ForwardIt next = first;
    ++next;

    while (next != last) {
        if (comp(*next, *first)) {
            return false;
        }
        first = next;
        ++next;
    }

    return true;
}

} // namespace lab4
