#pragma once

#include <utility>
#include "graph/EdgeNode.hpp"

namespace lab4 {

template <typename T>
struct VertexNode {
    T value;
    VertexNode* prevVertex;
    VertexNode* nextVertex;
    EdgeNode<T>* firstEdge;

    VertexNode(const T& val)
        : value(val), prevVertex(nullptr), nextVertex(nullptr), firstEdge(nullptr) {}

    VertexNode(T&& val)
        : value(std::move(val)), prevVertex(nullptr), nextVertex(nullptr), firstEdge(nullptr) {}
};

} // namespace lab4
