#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include "graph/VertexNode.hpp"

namespace lab4 {

template <typename T>
class Graph;

template <typename T>
class VertexIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    VertexNode<T>* node_;
    const Graph<T>* graph_;

    template <typename U>
    friend class Graph;
    template <typename U>
    friend class ConstVertexIterator;

public:
    VertexIterator() : node_(nullptr), graph_(nullptr) {}
    VertexIterator(VertexNode<T>* node, const Graph<T>* graph) : node_(node), graph_(graph) {}

    reference operator*() const {
        if (node_ == nullptr) {
            throw std::out_of_range("Dereferencing end iterator");
        }
        return node_->value;
    }

    pointer operator->() const {
        if (node_ == nullptr) {
            throw std::out_of_range("Dereferencing end iterator");
        }
        return &(node_->value);
    }

    VertexIterator& operator++() {
        if (node_ != nullptr) {
            node_ = node_->nextVertex;
        }
        return *this;
    }

    VertexIterator operator++(int) {
        VertexIterator temp = *this;
        ++(*this);
        return temp;
    }

    VertexIterator& operator--() {
        if (node_ == nullptr) {
            if (graph_ != nullptr) {
                node_ = graph_->tail_;
            }
        } else {
            node_ = node_->prevVertex;
        }
        return *this;
    }

    VertexIterator operator--(int) {
        VertexIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const VertexIterator& other) const {
        return node_ == other.node_ && graph_ == other.graph_;
    }

    bool operator!=(const VertexIterator& other) const {
        return !(*this == other);
    }
};

template <typename T>
class ConstVertexIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    const VertexNode<T>* node_;
    const Graph<T>* graph_;

    template <typename U>
    friend class Graph;

public:
    ConstVertexIterator() : node_(nullptr), graph_(nullptr) {}
    ConstVertexIterator(const VertexNode<T>* node, const Graph<T>* graph)
        : node_(node), graph_(graph) {}
    ConstVertexIterator(const VertexIterator<T>& it)
        : node_(it.node_), graph_(it.graph_) {}

    reference operator*() const {
        if (node_ == nullptr) {
            throw std::out_of_range("Dereferencing end const_iterator");
        }
        return node_->value;
    }

    pointer operator->() const {
        if (node_ == nullptr) {
            throw std::out_of_range("Dereferencing end const_iterator");
        }
        return &(node_->value);
    }

    ConstVertexIterator& operator++() {
        if (node_ != nullptr) {
            node_ = node_->nextVertex;
        }
        return *this;
    }

    ConstVertexIterator operator++(int) {
        ConstVertexIterator temp = *this;
        ++(*this);
        return temp;
    }

    ConstVertexIterator& operator--() {
        if (node_ == nullptr) {
            if (graph_ != nullptr) {
                node_ = graph_->tail_;
            }
        } else {
            node_ = node_->prevVertex;
        }
        return *this;
    }

    ConstVertexIterator operator--(int) {
        ConstVertexIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const ConstVertexIterator& other) const {
        return node_ == other.node_ && graph_ == other.graph_;
    }

    bool operator!=(const ConstVertexIterator& other) const {
        return !(*this == other);
    }
};

} // namespace lab4
