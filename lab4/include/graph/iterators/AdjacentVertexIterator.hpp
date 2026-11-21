#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include "graph/VertexNode.hpp"
#include "graph/EdgeNode.hpp"

namespace lab4 {

template <typename T>
class AdjacentVertexIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    const VertexNode<T>* origin_;
    const EdgeNode<T>* edge_;

    template <typename U>
    friend class Graph;

public:
    AdjacentVertexIterator() : origin_(nullptr), edge_(nullptr) {}
    AdjacentVertexIterator(const VertexNode<T>* origin, const EdgeNode<T>* edge)
        : origin_(origin), edge_(edge) {}

    reference operator*() const {
        if (edge_ == nullptr || edge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end adjacent_vertex_iterator");
        }
        return edge_->target->value;
    }

    pointer operator->() const {
        if (edge_ == nullptr || edge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end adjacent_vertex_iterator");
        }
        return &(edge_->target->value);
    }

    AdjacentVertexIterator& operator++() {
        if (edge_ != nullptr) {
            edge_ = edge_->nextEdge;
        }
        return *this;
    }

    AdjacentVertexIterator operator++(int) {
        AdjacentVertexIterator temp = *this;
        ++(*this);
        return temp;
    }

    AdjacentVertexIterator& operator--() {
        if (edge_ == nullptr) {
            if (origin_ != nullptr) {
                const EdgeNode<T>* curr = origin_->firstEdge;
                while (curr != nullptr && curr->nextEdge != nullptr) {
                    curr = curr->nextEdge;
                }
                edge_ = curr;
            }
        } else {
            edge_ = edge_->prevEdge;
        }
        return *this;
    }

    AdjacentVertexIterator operator--(int) {
        AdjacentVertexIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const AdjacentVertexIterator& other) const {
        return origin_ == other.origin_ && edge_ == other.edge_;
    }

    bool operator!=(const AdjacentVertexIterator& other) const {
        return !(*this == other);
    }
};

} // namespace lab4
