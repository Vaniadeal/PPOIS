#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>

#include "graph/VertexNode.hpp"
#include "graph/EdgeNode.hpp"
#include "graph/iterators/VertexIterator.hpp"
#include "graph/iterators/AdjacentVertexIterator.hpp"
#include "graph/iterators/ForwardAdjacentVertexIterator.hpp"
#include "graph/iterators/IncidentEdgeIterator.hpp"
#include "graph/iterators/EdgeIterator.hpp"

namespace lab4 {

template <typename T>
class Graph {
private:
    VertexNode<T>* head_;
    VertexNode<T>* tail_;
    std::size_t vertexCount_;
    std::size_t edgeCount_;

    template <typename U>
    friend class VertexIterator;
    template <typename U>
    friend class ConstVertexIterator;
    template <typename U>
    friend class EdgeIterator;

    VertexNode<T>* findVertexNode(const T& val) const {
        VertexNode<T>* curr = head_;
        while (curr != nullptr) {
            if (curr->value == val) {
                return curr;
            }
            curr = curr->nextVertex;
        }
        return nullptr;
    }

    EdgeNode<T>* findEdgeNode(VertexNode<T>* from, VertexNode<T>* to) const {
        if (from == nullptr || to == nullptr) {
            return nullptr;
        }
        EdgeNode<T>* edge = from->firstEdge;
        while (edge != nullptr) {
            if (edge->target == to) {
                return edge;
            }
            edge = edge->nextEdge;
        }
        return nullptr;
    }

    void removeEdgeNodes(VertexNode<T>* u, VertexNode<T>* v) {
        EdgeNode<T>* e1 = findEdgeNode(u, v);
        if (e1 != nullptr) {
            if (e1->prevEdge != nullptr) {
                e1->prevEdge->nextEdge = e1->nextEdge;
            } else {
                u->firstEdge = e1->nextEdge;
            }
            if (e1->nextEdge != nullptr) {
                e1->nextEdge->prevEdge = e1->prevEdge;
            }
            delete e1;
        }

        EdgeNode<T>* e2 = findEdgeNode(v, u);
        if (e2 != nullptr) {
            if (e2->prevEdge != nullptr) {
                e2->prevEdge->nextEdge = e2->nextEdge;
            } else {
                v->firstEdge = e2->nextEdge;
            }
            if (e2->nextEdge != nullptr) {
                e2->nextEdge->prevEdge = e2->prevEdge;
            }
            delete e2;
        }
    }

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using edge_type = std::pair<T, T>;

    using iterator = VertexIterator<T>;
    using const_iterator = ConstVertexIterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using adjacent_vertex_iterator = AdjacentVertexIterator<T>;
    using const_adjacent_vertex_iterator = AdjacentVertexIterator<T>;
    using reverse_adjacent_vertex_iterator = std::reverse_iterator<adjacent_vertex_iterator>;
    using const_reverse_adjacent_vertex_iterator = std::reverse_iterator<const_adjacent_vertex_iterator>;

    using forward_adjacent_vertex_iterator = ForwardAdjacentVertexIterator<T>;

    using incident_edge_iterator = IncidentEdgeIterator<T>;
    using const_incident_edge_iterator = IncidentEdgeIterator<T>;
    using reverse_incident_edge_iterator = std::reverse_iterator<incident_edge_iterator>;
    using const_reverse_incident_edge_iterator = std::reverse_iterator<const_incident_edge_iterator>;

    using edge_iterator = EdgeIterator<T>;
    using const_edge_iterator = EdgeIterator<T>;
    using reverse_edge_iterator = std::reverse_iterator<edge_iterator>;
    using const_reverse_edge_iterator = std::reverse_iterator<const_edge_iterator>;

    Graph() : head_(nullptr), tail_(nullptr), vertexCount_(0), edgeCount_(0) {}

    Graph(std::initializer_list<T> vertices) : Graph() {
        for (const auto& v : vertices) {
            add_vertex(v);
        }
    }

    Graph(const Graph& other) : Graph() {
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
            add_vertex(*it);
        }
        for (auto it = other.edge_cbegin(); it != other.edge_cend(); ++it) {
            add_edge((*it).first, (*it).second);
        }
    }

    Graph(Graph&& other) noexcept
        : head_(other.head_), tail_(other.tail_),
          vertexCount_(other.vertexCount_), edgeCount_(other.edgeCount_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.vertexCount_ = 0;
        other.edgeCount_ = 0;
    }

    ~Graph() {
        clear();
    }

    Graph& operator=(const Graph& other) {
        if (this != &other) {
            Graph temp(other);
            swap(temp);
        }
        return *this;
    }

    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            vertexCount_ = other.vertexCount_;
            edgeCount_ = other.edgeCount_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.vertexCount_ = 0;
            other.edgeCount_ = 0;
        }
        return *this;
    }

    void swap(Graph& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(vertexCount_, other.vertexCount_);
        std::swap(edgeCount_, other.edgeCount_);
    }

    bool empty() const noexcept {
        return vertexCount_ == 0;
    }

    size_type vertex_count() const noexcept {
        return vertexCount_;
    }

    size_type edge_count() const noexcept {
        return edgeCount_;
    }

    void clear() noexcept {
        VertexNode<T>* vCurr = head_;
        while (vCurr != nullptr) {
            EdgeNode<T>* eCurr = vCurr->firstEdge;
            while (eCurr != nullptr) {
                EdgeNode<T>* eNext = eCurr->nextEdge;
                delete eCurr;
                eCurr = eNext;
            }
            VertexNode<T>* vNext = vCurr->nextVertex;
            delete vCurr;
            vCurr = vNext;
        }
        head_ = nullptr;
        tail_ = nullptr;
        vertexCount_ = 0;
        edgeCount_ = 0;
    }

    bool has_vertex(const T& val) const {
        return findVertexNode(val) != nullptr;
    }

    bool has_edge(const T& u, const T& v) const {
        VertexNode<T>* uNode = findVertexNode(u);
        VertexNode<T>* vNode = findVertexNode(v);
        return findEdgeNode(uNode, vNode) != nullptr;
    }

    bool add_vertex(const T& val) {
        if (has_vertex(val)) {
            return false;
        }
        VertexNode<T>* newNode = new VertexNode<T>(val);
        if (head_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
        } else {
            tail_->nextVertex = newNode;
            newNode->prevVertex = tail_;
            tail_ = newNode;
        }
        ++vertexCount_;
        return true;
    }

    bool add_vertex(T&& val) {
        if (has_vertex(val)) {
            return false;
        }
        VertexNode<T>* newNode = new VertexNode<T>(std::move(val));
        if (head_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
        } else {
            tail_->nextVertex = newNode;
            newNode->prevVertex = tail_;
            tail_ = newNode;
        }
        ++vertexCount_;
        return true;
    }

    bool add_edge(const T& u, const T& v) {
        if (u == v) {
            throw std::invalid_argument("Self-loops are not allowed in this simple graph");
        }
        VertexNode<T>* uNode = findVertexNode(u);
        VertexNode<T>* vNode = findVertexNode(v);
        if (uNode == nullptr || vNode == nullptr) {
            throw std::invalid_argument("One or both vertices not found in graph");
        }
        if (findEdgeNode(uNode, vNode) != nullptr) {
            return false;
        }

        EdgeNode<T>* edgeUtoV = new EdgeNode<T>(vNode);
        if (uNode->firstEdge == nullptr) {
            uNode->firstEdge = edgeUtoV;
        } else {
            EdgeNode<T>* curr = uNode->firstEdge;
            while (curr->nextEdge != nullptr) {
                curr = curr->nextEdge;
            }
            curr->nextEdge = edgeUtoV;
            edgeUtoV->prevEdge = curr;
        }

        EdgeNode<T>* edgeVtoU = new EdgeNode<T>(uNode);
        if (vNode->firstEdge == nullptr) {
            vNode->firstEdge = edgeVtoU;
        } else {
            EdgeNode<T>* curr = vNode->firstEdge;
            while (curr->nextEdge != nullptr) {
                curr = curr->nextEdge;
            }
            curr->nextEdge = edgeVtoU;
            edgeVtoU->prevEdge = curr;
        }

        ++edgeCount_;
        return true;
    }

    bool remove_edge(const T& u, const T& v) {
        VertexNode<T>* uNode = findVertexNode(u);
        VertexNode<T>* vNode = findVertexNode(v);
        if (uNode == nullptr || vNode == nullptr) {
            return false;
        }
        if (findEdgeNode(uNode, vNode) == nullptr) {
            return false;
        }

        removeEdgeNodes(uNode, vNode);
        --edgeCount_;
        return true;
    }

    bool remove_vertex(const T& val) {
        VertexNode<T>* vNode = findVertexNode(val);
        if (vNode == nullptr) {
            return false;
        }

        EdgeNode<T>* eCurr = vNode->firstEdge;
        while (eCurr != nullptr) {
            VertexNode<T>* target = eCurr->target;
            EdgeNode<T>* backEdge = findEdgeNode(target, vNode);
            if (backEdge != nullptr) {
                if (backEdge->prevEdge != nullptr) {
                    backEdge->prevEdge->nextEdge = backEdge->nextEdge;
                } else {
                    target->firstEdge = backEdge->nextEdge;
                }
                if (backEdge->nextEdge != nullptr) {
                    backEdge->nextEdge->prevEdge = backEdge->prevEdge;
                }
                delete backEdge;
            }
            --edgeCount_;
            EdgeNode<T>* next = eCurr->nextEdge;
            delete eCurr;
            eCurr = next;
        }
        vNode->firstEdge = nullptr;

        if (vNode->prevVertex != nullptr) {
            vNode->prevVertex->nextVertex = vNode->nextVertex;
        } else {
            head_ = vNode->nextVertex;
        }

        if (vNode->nextVertex != nullptr) {
            vNode->nextVertex->prevVertex = vNode->prevVertex;
        } else {
            tail_ = vNode->prevVertex;
        }

        delete vNode;
        --vertexCount_;
        return true;
    }

    size_type degree(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        size_type count = 0;
        EdgeNode<T>* edge = node->firstEdge;
        while (edge != nullptr) {
            ++count;
            edge = edge->nextEdge;
        }
        return count;
    }

    size_type edge_degree(const T& u, const T& v) const {
        if (!has_edge(u, v)) {
            throw std::invalid_argument("Edge not found in graph");
        }
        return degree(u) + degree(v) - 2;
    }

    iterator erase(const iterator& pos) {
        if (pos.node_ == nullptr) {
            throw std::out_of_range("Cannot erase end iterator");
        }
        VertexNode<T>* nextNode = pos.node_->nextVertex;
        remove_vertex(pos.node_->value);
        return iterator(nextNode, this);
    }

    edge_iterator erase(const edge_iterator& pos) {
        if (pos.currentVertex_ == nullptr || pos.currentEdge_ == nullptr) {
            throw std::out_of_range("Cannot erase end edge iterator");
        }
        T u = pos.currentVertex_->value;
        T v = pos.currentEdge_->target->value;
        edge_iterator nextIt = pos;
        ++nextIt;
        remove_edge(u, v);
        return nextIt;
    }

    iterator begin() noexcept {
        return iterator(head_, this);
    }

    iterator end() noexcept {
        return iterator(nullptr, this);
    }

    const_iterator begin() const noexcept {
        return const_iterator(head_, this);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr, this);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head_, this);
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr, this);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    edge_iterator edge_begin() const noexcept {
        if (head_ == nullptr) {
            return edge_iterator(this, nullptr, nullptr);
        }
        return edge_iterator(this, head_, head_->firstEdge);
    }

    edge_iterator edge_end() const noexcept {
        return edge_iterator(this, nullptr, nullptr);
    }

    const_edge_iterator edge_cbegin() const noexcept {
        return edge_begin();
    }

    const_edge_iterator edge_cend() const noexcept {
        return edge_end();
    }

    reverse_edge_iterator edge_rbegin() const noexcept {
        return reverse_edge_iterator(edge_end());
    }

    reverse_edge_iterator edge_rend() const noexcept {
        return reverse_edge_iterator(edge_begin());
    }

    const_reverse_edge_iterator edge_crbegin() const noexcept {
        return const_reverse_edge_iterator(edge_cend());
    }

    const_reverse_edge_iterator edge_crend() const noexcept {
        return const_reverse_edge_iterator(edge_cbegin());
    }

    adjacent_vertex_iterator adjacent_vertex_begin(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return adjacent_vertex_iterator(node, node->firstEdge);
    }

    adjacent_vertex_iterator adjacent_vertex_end(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return adjacent_vertex_iterator(node, nullptr);
    }

    forward_adjacent_vertex_iterator forward_adjacent_begin(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return forward_adjacent_vertex_iterator(node->firstEdge);
    }

    forward_adjacent_vertex_iterator forward_adjacent_end(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return forward_adjacent_vertex_iterator(nullptr);
    }

    incident_edge_iterator incident_edge_begin(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return incident_edge_iterator(node, node->firstEdge);
    }

    incident_edge_iterator incident_edge_end(const T& val) const {
        VertexNode<T>* node = findVertexNode(val);
        if (node == nullptr) {
            throw std::invalid_argument("Vertex not found in graph");
        }
        return incident_edge_iterator(node, nullptr);
    }

    bool operator==(const Graph& other) const {
        if (vertexCount_ != other.vertexCount_ || edgeCount_ != other.edgeCount_) {
            return false;
        }
        for (auto it = cbegin(); it != cend(); ++it) {
            if (!other.has_vertex(*it)) {
                return false;
            }
        }
        for (auto it = edge_cbegin(); it != edge_cend(); ++it) {
            if (!other.has_edge((*it).first, (*it).second)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Graph& other) const {
        return !(*this == other);
    }

    bool operator<(const Graph& other) const {
        if (vertexCount_ != other.vertexCount_) {
            return vertexCount_ < other.vertexCount_;
        }
        return edgeCount_ < other.edgeCount_;
    }

    bool operator>(const Graph& other) const {
        return other < *this;
    }

    bool operator<=(const Graph& other) const {
        return !(other < *this);
    }

    bool operator>=(const Graph& other) const {
        return !(*this < other);
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Graph<U>& graph);
};

template <typename U>
std::ostream& operator<<(std::ostream& os, const Graph<U>& graph) {
    os << "Graph(vertices: " << graph.vertex_count() << ", edges: " << graph.edge_count() << ")\n";
    os << "Vertices: [";
    bool firstV = true;
    std::for_each(graph.cbegin(), graph.cend(), [&os, &firstV](const U& vertex) {
        if (!firstV) {
            os << ", ";
        }
        os << vertex;
        firstV = false;
    });
    os << "]\nEdges:\n";
    std::for_each(graph.edge_cbegin(), graph.edge_cend(), [&os](const std::pair<U, U>& edge) {
        os << "  (" << edge.first << " <-> " << edge.second << ")\n";
    });
    return os;
}

} // namespace lab4
