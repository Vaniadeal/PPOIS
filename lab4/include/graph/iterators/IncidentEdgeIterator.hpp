#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "graph/VertexNode.hpp"
#include "graph/EdgeNode.hpp"

namespace lab4 {

template <typename T>
class IncidentEdgeIterator {
public:
    using edge_type = std::pair<T, T>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = edge_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const edge_type*;
    using reference = edge_type;

private:
    const VertexNode<T>* origin_;
    const EdgeNode<T>* edge_;
    mutable edge_type currentPair_;

    template <typename U>
    friend class Graph;

public:
    IncidentEdgeIterator() : origin_(nullptr), edge_(nullptr) {}
    IncidentEdgeIterator(const VertexNode<T>* origin, const EdgeNode<T>* edge)
        : origin_(origin), edge_(edge) {}

    reference operator*() const {
        if (origin_ == nullptr || edge_ == nullptr || edge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end incident_edge_iterator");
        }
        currentPair_ = {origin_->value, edge_->target->value};
        return currentPair_;
    }

    IncidentEdgeIterator& operator++() {
        if (edge_ != nullptr) {
            edge_ = edge_->nextEdge;
        }
        return *this;
    }

    IncidentEdgeIterator operator++(int) {
        IncidentEdgeIterator temp = *this;
        ++(*this);
        return temp;
    }

    IncidentEdgeIterator& operator--() {
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

    IncidentEdgeIterator operator--(int) {
        IncidentEdgeIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const IncidentEdgeIterator& other) const {
        return origin_ == other.origin_ && edge_ == other.edge_;
    }

    bool operator!=(const IncidentEdgeIterator& other) const {
        return !(*this == other);
    }
};

} // namespace lab4
