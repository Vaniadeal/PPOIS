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
    Graph<int> g;
    int firstVal = 10;
    CHECK(g.add_vertex(std::move(firstVal)));
    CHECK_EQUAL(1, g.vertex_count());

    CHECK(g.add_vertex(20));
    CHECK(!g.add_vertex(10));
    CHECK_EQUAL(2, g.vertex_count());

    int duplicateVal = 20;
    CHECK(!g.add_vertex(std::move(duplicateVal)));

    int thirdVal = 30;
    CHECK(g.add_vertex(std::move(thirdVal)));
    CHECK_EQUAL(3, g.vertex_count());
}

TEST(AddEdge_BasicAndInvalid) {
    Graph<int> g = {1, 2, 3};
    CHECK(g.add_edge(1, 2));
    CHECK_EQUAL(1, g.edge_count());
    CHECK(g.has_edge(1, 2));
    CHECK(g.has_edge(2, 1));
    CHECK(!g.has_edge(1, 3));
    CHECK(!g.has_edge(1, 99));

    CHECK(!g.add_edge(1, 2));
    CHECK(!g.add_edge(2, 1));

    CHECK_THROW(g.add_edge(1, 1), std::invalid_argument);
    CHECK_THROW(g.add_edge(1, 99), std::invalid_argument);
    CHECK_THROW(g.add_edge(99, 1), std::invalid_argument);
}

TEST(RemoveEdge_DecrementsCount) {
    Graph<int> g = {1, 2, 3, 4};
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    CHECK_EQUAL(3, g.edge_count());

    CHECK(g.remove_edge(1, 3));
    CHECK_EQUAL(2, g.edge_count());
    CHECK(!g.has_edge(1, 3));
    CHECK(!g.has_edge(3, 1));

    CHECK(g.remove_edge(1, 2));
    CHECK_EQUAL(1, g.edge_count());

    CHECK(g.remove_edge(1, 4));
    CHECK_EQUAL(0, g.edge_count());

    CHECK(!g.remove_edge(1, 2));
    CHECK(!g.remove_edge(99, 100));
}

TEST(RemoveVertex_RemovesIncidentEdges) {
    Graph<int> g = {1, 2, 3, 4, 5};
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(2, 4);
    CHECK_EQUAL(5, g.edge_count());

    CHECK(g.remove_vertex(3));
    CHECK_EQUAL(4, g.vertex_count());
    CHECK_EQUAL(3, g.edge_count());
    CHECK(!g.has_vertex(3));
    CHECK(!g.has_edge(2, 3));
    CHECK(!g.has_edge(3, 4));
    CHECK(g.has_edge(2, 4));

    CHECK(g.remove_vertex(1));
    CHECK_EQUAL(3, g.vertex_count());

    CHECK(g.remove_vertex(5));
    CHECK_EQUAL(2, g.vertex_count());

    Graph<int> gSingle = {100};
    CHECK(gSingle.remove_vertex(100));
    CHECK_EQUAL(0, gSingle.vertex_count());
    CHECK(gSingle.empty());

    CHECK(!g.remove_vertex(3));
    CHECK(!g.remove_vertex(999));
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
    auto it = g.begin();
    CHECK_EQUAL(10, *it);
    CHECK_EQUAL(10, *it.operator->());
    auto postIt = it++;
    CHECK_EQUAL(10, *postIt);
    CHECK_EQUAL(20, *it);
    CHECK(it != g.end());

    auto revIt = g.end();
    auto postRev = revIt--;
    CHECK(postRev == g.end());
    CHECK_EQUAL(30, *revIt);

    const Graph<int>& constG = g;
    auto cIt = constG.cbegin();
    CHECK_EQUAL(10, *cIt);
    CHECK_EQUAL(10, *cIt.operator->());
    auto postCIt = cIt++;
    CHECK_EQUAL(10, *postCIt);
    CHECK_EQUAL(20, *cIt);
    CHECK(cIt != constG.cend());

    auto cRevIt = constG.cend();
    auto postCRev = cRevIt--;
    CHECK(postCRev == constG.cend());
    CHECK_EQUAL(30, *cRevIt);

    std::vector<int> revList;
    for (auto r = g.rbegin(); r != g.rend(); ++r) {
        revList.push_back(*r);
    }
    CHECK_EQUAL(3, revList.size());
    CHECK_EQUAL(30, revList[0]);
    CHECK_EQUAL(20, revList[1]);
    CHECK_EQUAL(10, revList[2]);

    std::vector<int> constRevList;
    for (auto cr = constG.crbegin(); cr != constG.crend(); ++cr) {
        constRevList.push_back(*cr);
    }
    CHECK_EQUAL(3, constRevList.size());
    CHECK_EQUAL(30, constRevList[0]);
}

TEST(EdgeIterator_BidirectionalAndReverse) {
    Graph<int> g = {1, 2, 3};
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);

    auto it = g.edge_begin();
    CHECK_EQUAL(1, (*it).first);
    CHECK_EQUAL(2, (*it).second);
    auto postIt = it++;
    CHECK_EQUAL(1, (*postIt).first);
    CHECK_EQUAL(2, (*postIt).second);
    CHECK_EQUAL(1, (*it).first);
    CHECK_EQUAL(3, (*it).second);
    CHECK(it != g.edge_end());

    auto endEdge = g.edge_end();
    auto postEnd = endEdge--;
    CHECK(postEnd == g.edge_end());

    std::vector<std::pair<int, int>> revEdges;
    for (auto r = g.edge_rbegin(); r != g.edge_rend(); ++r) {
        revEdges.push_back(*r);
    }
    CHECK_EQUAL(3, revEdges.size());
    CHECK_EQUAL(2, revEdges[0].first);
    CHECK_EQUAL(3, revEdges[0].second);

    std::vector<std::pair<int, int>> constRevEdges;
    for (auto cr = g.edge_crbegin(); cr != g.edge_crend(); ++cr) {
        constRevEdges.push_back(*cr);
    }
    CHECK_EQUAL(3, constRevEdges.size());
    CHECK_EQUAL(2, constRevEdges[0].first);
    CHECK_EQUAL(3, constRevEdges[0].second);
}

TEST(IncidentEdgeAndAdjacentVertexIterators) {
    Graph<int> g = {1, 2, 3};
    g.add_edge(1, 2);
    g.add_edge(1, 3);

    auto adjIt = g.adjacent_vertex_begin(1);
    CHECK_EQUAL(2, *adjIt);
    CHECK_EQUAL(2, *adjIt.operator->());
    auto postAdj = adjIt++;
    CHECK(postAdj != adjIt);
    auto adjEnd = g.adjacent_vertex_end(1);
    auto postAdjEnd = adjEnd--;
    CHECK(postAdjEnd == g.adjacent_vertex_end(1));

    std::vector<int> revAdj;
    for (auto it = g.adjacent_vertex_end(1); it != g.adjacent_vertex_begin(1); ) {
        --it;
        revAdj.push_back(*it);
    }
    CHECK_EQUAL(2, revAdj.size());

    auto incIt = g.incident_edge_begin(1);
    CHECK_EQUAL(1, (*incIt).first);
    auto postInc = incIt++;
    CHECK(postInc != incIt);
    auto incEnd = g.incident_edge_end(1);
    auto postIncEnd = incEnd--;
    CHECK(postIncEnd == g.incident_edge_end(1));

    CHECK_THROW(g.adjacent_vertex_begin(999), std::invalid_argument);
    CHECK_THROW(g.adjacent_vertex_end(999), std::invalid_argument);
    CHECK_THROW(g.incident_edge_begin(999), std::invalid_argument);
    CHECK_THROW(g.incident_edge_end(999), std::invalid_argument);
}

TEST(Variant11_ForwardAdjacentVertexIterator) {
    Graph<int> g = {100, 200, 300};
    g.add_edge(100, 200);
    g.add_edge(100, 300);

    auto it = g.forward_adjacent_begin(100);
    CHECK_EQUAL(200, *it);
    CHECK_EQUAL(200, *it.operator->());
    auto post = it++;
    CHECK_EQUAL(200, *post);
    CHECK_EQUAL(300, *it);
    CHECK(it != g.forward_adjacent_end(100));
    ++it;
    CHECK(it == g.forward_adjacent_end(100));

    CHECK_THROW(it.operator->(), std::out_of_range);
    CHECK_THROW(*it, std::out_of_range);
    CHECK_THROW(g.forward_adjacent_begin(999), std::invalid_argument);
    CHECK_THROW(g.forward_adjacent_end(999), std::invalid_argument);
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

    CHECK_THROW(g.erase(g.end()), std::out_of_range);
    CHECK_THROW(g2.erase(g2.edge_end()), std::out_of_range);
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
    assigned = assigned;
    CHECK_EQUAL(3, assigned.vertex_count());

    Graph<int> moveAssigned;
    moveAssigned = std::move(assigned);
    CHECK_EQUAL(3, moveAssigned.vertex_count());
    moveAssigned = std::move(moveAssigned);
    CHECK_EQUAL(3, moveAssigned.vertex_count());

    Graph<int> gSwap1 = {1};
    Graph<int> gSwap2 = {2, 3};
    gSwap1.swap(gSwap2);
    CHECK_EQUAL(2, gSwap1.vertex_count());
    CHECK_EQUAL(1, gSwap2.vertex_count());
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

    Graph<int> g4 = {1, 3};
    g4.add_edge(1, 3);
    CHECK(!(g1 == g4));

    Graph<int> g5 = {1, 2};
    CHECK(!(g1 == g5));
    CHECK(!(g5 == g1));
}

TEST(StreamOutputOperator) {
    Graph<int> g = {10, 20};
    g.add_edge(10, 20);
    std::ostringstream oss;
    oss << g;
    std::string output = oss.str();
    CHECK(output.find("Graph(vertices: 2, edges: 1)") != std::string::npos);
    CHECK(output.find("10") != std::string::npos);
    CHECK(output.find("20") != std::string::npos);
}

TEST(IteratorDereferenceExceptions) {
    Graph<int> g = {1};
    auto endIt = g.end();
    CHECK_THROW(*endIt, std::out_of_range);
    CHECK_THROW(endIt.operator->(), std::out_of_range);

    auto endConstIt = g.cend();
    CHECK_THROW(*endConstIt, std::out_of_range);
    CHECK_THROW(endConstIt.operator->(), std::out_of_range);

    auto endEdge = g.edge_end();
    CHECK_THROW(*endEdge, std::out_of_range);

    auto endAdj = g.adjacent_vertex_end(1);
    CHECK_THROW(*endAdj, std::out_of_range);
    CHECK_THROW(endAdj.operator->(), std::out_of_range);

    auto endInc = g.incident_edge_end(1);
    CHECK_THROW(*endInc, std::out_of_range);
}

}

int main() {
    return UnitTest::RunAllTests();
}
