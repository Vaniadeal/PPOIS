#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "graph/VertexNode.hpp"
#include "graph/EdgeNode.hpp"

namespace lab4 {

template <typename T>
class Graph;

template <typename T>
class EdgeIterator {
public:
    using edge_type = std::pair<T, T>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = edge_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const edge_type*;
    using reference = edge_type;

private:
    const Graph<T>* graph_;
    const VertexNode<T>* currentVertex_;
    const EdgeNode<T>* currentEdge_;
    mutable edge_type currentPair_;

    template <typename U>
    friend class Graph;

    void advanceToValid() {
        while (currentVertex_ != nullptr) {
            while (currentEdge_ != nullptr) {
                if (currentVertex_->value < currentEdge_->target->value) {
                    return;
                }
                currentEdge_ = currentEdge_->nextEdge;
            }
            currentVertex_ = currentVertex_->nextVertex;
            if (currentVertex_ != nullptr) {
                currentEdge_ = currentVertex_->firstEdge;
            } else {
                currentEdge_ = nullptr;
            }
        }
    }

    void retreatToValid() {
        if (currentVertex_ == nullptr) {
            if (graph_ == nullptr || graph_->tail_ == nullptr) {
                return;
            }
            currentVertex_ = graph_->tail_;
            const EdgeNode<T>* lastEdge = currentVertex_->firstEdge;
            while (lastEdge != nullptr && lastEdge->nextEdge != nullptr) {
                lastEdge = lastEdge->nextEdge;
            }
            currentEdge_ = lastEdge;
        } else {
            if (currentEdge_ != nullptr) {
                currentEdge_ = currentEdge_->prevEdge;
            }
        }

        while (currentVertex_ != nullptr) {
            while (currentEdge_ != nullptr) {
                if (currentVertex_->value < currentEdge_->target->value) {
                    return;
                }
                currentEdge_ = currentEdge_->prevEdge;
            }
            currentVertex_ = currentVertex_->prevVertex;
            if (currentVertex_ != nullptr) {
                const EdgeNode<T>* lastEdge = currentVertex_->firstEdge;
                while (lastEdge != nullptr && lastEdge->nextEdge != nullptr) {
                    lastEdge = lastEdge->nextEdge;
                }
                currentEdge_ = lastEdge;
            } else {
                currentEdge_ = nullptr;
            }
        }
    }

public:
    EdgeIterator() : graph_(nullptr), currentVertex_(nullptr), currentEdge_(nullptr) {}
    EdgeIterator(const Graph<T>* graph, const VertexNode<T>* vNode, const EdgeNode<T>* eNode)
        : graph_(graph), currentVertex_(vNode), currentEdge_(eNode) {
        advanceToValid();
    }

    reference operator*() const {
        if (currentVertex_ == nullptr || currentEdge_ == nullptr || currentEdge_->target == nullptr) {
            throw std::out_of_range("Dereferencing end edge_iterator");
        }
        currentPair_ = {currentVertex_->value, currentEdge_->target->value};
        return currentPair_;
    }

    EdgeIterator& operator++() {
        if (currentEdge_ != nullptr) {
            currentEdge_ = currentEdge_->nextEdge;
        }
        advanceToValid();
        return *this;
    }

    EdgeIterator operator++(int) {
        EdgeIterator temp = *this;
        ++(*this);
        return temp;
    }

    EdgeIterator& operator--() {
        retreatToValid();
        return *this;
    }

    EdgeIterator operator--(int) {
        EdgeIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const EdgeIterator& other) const {
        return currentVertex_ == other.currentVertex_ && currentEdge_ == other.currentEdge_;
    }

    bool operator!=(const EdgeIterator& other) const {
        return !(*this == other);
    }
};

} // namespace lab4
