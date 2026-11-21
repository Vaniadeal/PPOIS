#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include "graph/VertexNode.hpp"
#include "graph/EdgeNode.hpp"

namespace lab4 {

template <typename T>
class ForwardAdjacentVertexIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    const EdgeNode<T>* edge_;

    template <typename U>
    friend class Graph;

public:
    ForwardAdjacentVertexIterator() : edge_(nullptr) {}
    explicit ForwardAdjacentVertexIterator(const EdgeNode<T>* edge) : edge_(edge) {}

    reference operator*() const {
        if (edge_ == nullptr || edge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end forward_adjacent_vertex_iterator");
        }
        return edge_->target->value;
    }

    pointer operator->() const {
        if (edge_ == nullptr || edge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end forward_adjacent_vertex_iterator");
        }
        return &(edge_->target->value);
    }

    ForwardAdjacentVertexIterator& operator++() {
        if (edge_ != nullptr) {
            edge_ = edge_->nextEdge;
        }
        return *this;
    }

    ForwardAdjacentVertexIterator operator++(int) {
        ForwardAdjacentVertexIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const ForwardAdjacentVertexIterator& other) const {
        return edge_ == other.edge_;
    }

    bool operator!=(const ForwardAdjacentVertexIterator& other) const {
        return !(*this == other);
    }
};

} // namespace lab4
