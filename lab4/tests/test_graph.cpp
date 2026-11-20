#include <UnitTest++/UnitTest++.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "graph.hpp"

using namespace lab4;

SUITE(GraphTests) {

TEST(DefaultConstructor_IsEmpty) {
    Graph<int> g;
    CHECK(g.empty());
    CHECK_EQUAL(0, g.vertex_count());
    CHECK_EQUAL(0, g.edge_count());
    CHECK(g.begin() == g.end());
    CHECK(g.edge_begin() == g.edge_end());
}

TEST(InitializerList_AddsVertices) {
    Graph<int> g = {10, 20, 30};
    CHECK(!g.empty());
    CHECK_EQUAL(3, g.vertex_count());
    CHECK_EQUAL(0, g.edge_count());
    CHECK(g.has_vertex(10));
    CHECK(g.has_vertex(20));
    CHECK(g.has_vertex(30));
    CHECK(!g.has_vertex(40));
}

TEST(AddVertex_DuplicateReturnsFalse) {
    Graph<std::string> g;
    CHECK(g.add_vertex("Minsk"));
    CHECK(g.add_vertex("Brest"));
    CHECK(!g.add_vertex("Minsk"));
    CHECK_EQUAL(2, g.vertex_count());
}

TEST(AddEdge_BasicAndInvalid) {
    Graph<int> g = {1, 2, 3};
    CHECK(g.add_edge(1, 2));
    CHECK_EQUAL(1, g.edge_count());
    CHECK(g.has_edge(1, 2));
    CHECK(g.has_edge(2, 1));
    CHECK(!g.has_edge(1, 3));

    CHECK(!g.add_edge(1, 2));
    CHECK(!g.add_edge(2, 1));

    CHECK_THROW(g.add_edge(1, 1), std::invalid_argument);
    CHECK_THROW(g.add_edge(1, 99), std::invalid_argument);
}

TEST(RemoveEdge_DecrementsCount) {
    Graph<int> g = {1, 2, 3};
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    CHECK_EQUAL(2, g.edge_count());

    CHECK(g.remove_edge(1, 2));
    CHECK_EQUAL(1, g.edge_count());
    CHECK(!g.has_edge(1, 2));
    CHECK(!g.has_edge(2, 1));
    CHECK(!g.remove_edge(1, 2));
}

TEST(RemoveVertex_RemovesIncidentEdges) {
    Graph<int> g = {1, 2, 3, 4};
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 3);
    CHECK_EQUAL(4, g.edge_count());

    CHECK(g.remove_vertex(1));
    CHECK_EQUAL(3, g.vertex_count());
    CHECK_EQUAL(1, g.edge_count());
    CHECK(!g.has_vertex(1));
    CHECK(!g.has_edge(1, 2));
    CHECK(g.has_edge(2, 3));
    CHECK(!g.remove_vertex(1));
}

TEST(DegreeAndEdgeDegree) {
    Graph<int> g = {1, 2, 3, 4};
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 3);

    CHECK_EQUAL(3, g.degree(1));
    CHECK_EQUAL(2, g.degree(2));
    CHECK_EQUAL(2, g.degree(3));
    CHECK_EQUAL(1, g.degree(4));

    CHECK_EQUAL(3, g.edge_degree(1, 2));
    CHECK_EQUAL(2, g.edge_degree(1, 4));

    CHECK_THROW(g.degree(99), std::invalid_argument);
    CHECK_THROW(g.edge_degree(1, 99), std::invalid_argument);
    CHECK_THROW(g.edge_degree(2, 4), std::invalid_argument);
}

TEST(VertexIterator_BidirectionalAndConst) {
    Graph<int> g = {10, 20, 30};
    std::vector<int> collected;
    for (auto it = g.begin(); it != g.end(); ++it) {
        collected.push_back(*it);
    }
    CHECK_EQUAL(3, collected.size());
    CHECK_EQUAL(10, collected[0]);
    CHECK_EQUAL(20, collected[1]);
    CHECK_EQUAL(30, collected[2]);

    auto revIt = g.end();
    --revIt;
    CHECK_EQUAL(30, *revIt);
    --revIt;
    CHECK_EQUAL(20, *revIt);
    --revIt;
    CHECK_EQUAL(10, *revIt);

    const Graph<int>& constG = g;
    std::vector<int> constCollected;
    for (auto it = constG.cbegin(); it != constG.cend(); ++it) {
        constCollected.push_back(*it);
    }
    CHECK_EQUAL(3, constCollected.size());

    std::vector<int> reverseCollected;
    for (auto it = g.rbegin(); it != g.rend(); ++it) {
        reverseCollected.push_back(*it);
    }
    CHECK_EQUAL(3, reverseCollected.size());
    CHECK_EQUAL(30, reverseCollected[0]);
    CHECK_EQUAL(20, reverseCollected[1]);
    CHECK_EQUAL(10, reverseCollected[2]);
}

TEST(EdgeIterator_BidirectionalAndReverse) {
    Graph<int> g = {1, 2, 3};
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);

    std::vector<std::pair<int, int>> edges;
    for (auto it = g.edge_begin(); it != g.edge_end(); ++it) {
        edges.push_back(*it);
    }
    CHECK_EQUAL(3, edges.size());
    CHECK_EQUAL(1, edges[0].first);
    CHECK_EQUAL(2, edges[0].second);
    CHECK_EQUAL(1, edges[1].first);
    CHECK_EQUAL(3, edges[1].second);
    CHECK_EQUAL(2, edges[2].first);
    CHECK_EQUAL(3, edges[2].second);

    std::vector<std::pair<int, int>> revEdges;
    for (auto it = g.edge_rbegin(); it != g.edge_rend(); ++it) {
        revEdges.push_back(*it);
    }
    CHECK_EQUAL(3, revEdges.size());
    CHECK_EQUAL(2, revEdges[0].first);
    CHECK_EQUAL(3, revEdges[0].second);
}

TEST(IncidentEdgeAndAdjacentVertexIterators) {
    Graph<std::string> g = {"A", "B", "C"};
    g.add_edge("A", "B");
    g.add_edge("A", "C");

    std::vector<std::string> adj;
    for (auto it = g.adjacent_vertex_begin("A"); it != g.adjacent_vertex_end("A"); ++it) {
        adj.push_back(*it);
    }
    CHECK_EQUAL(2, adj.size());

    std::vector<std::pair<std::string, std::string>> inc;
    for (auto it = g.incident_edge_begin("A"); it != g.incident_edge_end("A"); ++it) {
        inc.push_back(*it);
    }
    CHECK_EQUAL(2, inc.size());
}

TEST(Variant11_ForwardAdjacentVertexIterator) {
    Graph<int> g = {100, 200, 300};
    g.add_edge(100, 200);
    g.add_edge(100, 300);

    std::vector<int> forwardAdj;
    for (auto it = g.forward_adjacent_begin(100); it != g.forward_adjacent_end(100); ++it) {
        forwardAdj.push_back(*it);
    }
    CHECK_EQUAL(2, forwardAdj.size());

    CHECK_THROW(g.forward_adjacent_begin(999), std::invalid_argument);
}

TEST(EraseByIterators) {
    Graph<int> g = {1, 2, 3};
    g.add_edge(1, 2);
    g.add_edge(2, 3);

    auto vIt = g.begin();
    ++vIt;
    auto nextV = g.erase(vIt);
    CHECK_EQUAL(2, g.vertex_count());
    CHECK(!g.has_vertex(2));
    CHECK_EQUAL(3, *nextV);

    Graph<int> g2 = {10, 20, 30};
    g2.add_edge(10, 20);
    g2.add_edge(20, 30);
    auto eIt = g2.edge_begin();
    auto nextE = g2.erase(eIt);
    CHECK_EQUAL(1, g2.edge_count());
    CHECK_EQUAL(20, (*nextE).first);
    CHECK_EQUAL(30, (*nextE).second);
}

TEST(DeepCopyAndMoveSemantics) {
    Graph<int> original = {1, 2, 3};
    original.add_edge(1, 2);
    original.add_edge(2, 3);

    Graph<int> copy(original);
    CHECK_EQUAL(3, copy.vertex_count());
    CHECK_EQUAL(2, copy.edge_count());

    copy.add_vertex(4);
    copy.add_edge(1, 4);
    CHECK_EQUAL(4, copy.vertex_count());
    CHECK_EQUAL(3, original.vertex_count());
    CHECK(!original.has_vertex(4));

    Graph<int> assigned;
    assigned = original;
    CHECK_EQUAL(3, assigned.vertex_count());

    Graph<int> moved(std::move(copy));
    CHECK_EQUAL(4, moved.vertex_count());
    CHECK_EQUAL(0, copy.vertex_count());
    CHECK(copy.empty());
}

TEST(ComparisonOperators) {
    Graph<int> g1 = {1, 2};
    g1.add_edge(1, 2);

    Graph<int> g2 = {1, 2};
    g2.add_edge(1, 2);

    CHECK(g1 == g2);
    CHECK(!(g1 != g2));

    Graph<int> g3 = {1, 2, 3};
    CHECK(g1 < g3);
    CHECK(g3 > g1);
    CHECK(g1 <= g2);
    CHECK(g1 >= g2);
}

TEST(StreamOutputOperator) {
    Graph<std::string> g = {"Alpha", "Beta"};
    g.add_edge("Alpha", "Beta");
    std::ostringstream oss;
    oss << g;
    std::string output = oss.str();
    CHECK(output.find("Graph(vertices: 2, edges: 1)") != std::string::npos);
    CHECK(output.find("Alpha") != std::string::npos);
    CHECK(output.find("Beta") != std::string::npos);
}

TEST(IteratorDereferenceExceptions) {
    Graph<int> g = {1};
    auto endIt = g.end();
    CHECK_THROW(*endIt, std::out_of_range);
    CHECK_THROW(endIt.operator->(), std::out_of_range);

    auto endEdge = g.edge_end();
    CHECK_THROW(*endEdge, std::out_of_range);

    auto endAdj = g.adjacent_vertex_end(1);
    CHECK_THROW(*endAdj, std::out_of_range);
}

}

int main() {
    return UnitTest::RunAllTests();
}
