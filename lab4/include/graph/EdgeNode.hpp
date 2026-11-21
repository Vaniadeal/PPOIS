#pragma once

namespace lab4 {

template <typename T>
struct VertexNode;

template <typename T>
struct EdgeNode {
    VertexNode<T>* target;
    EdgeNode* prevEdge;
    EdgeNode* nextEdge;

    EdgeNode(VertexNode<T>* tgt)
        : target(tgt), prevEdge(nullptr), nextEdge(nullptr) {}
};

} // namespace lab4
