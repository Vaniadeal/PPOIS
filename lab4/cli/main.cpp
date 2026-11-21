#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "graph.hpp"
#include "sorts.hpp"
#include "student.hpp"

using namespace lab4;

void runSortDemos() {
    std::cout << "\n=== DEMO 1: In-place Merge Sort (std::vector<int>) ===\n";
    std::vector<int> numbers = {38, 27, 43, 3, 9, 82, 10};
    std::cout << "Original vector: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    in_place_merge_sort_container(numbers);
    std::cout << "Sorted ascending: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    in_place_merge_sort_container(numbers, std::greater<int>());
    std::cout << "Sorted descending: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n=== DEMO 2: In-place Merge Sort (Raw C-array) ===\n";
    int rawArr[6] = {50, 12, 85, 23, 7, 41};
    std::cout << "Original raw array: ";
    for (int n : rawArr) std::cout << n << " ";
    std::cout << "\n";

    in_place_merge_sort_container(rawArr);
    std::cout << "Sorted raw array: ";
    for (int n : rawArr) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n=== DEMO 3: Custom Class Student Sorting ===\n";
    std::vector<Student> students = {
        Student(104, "Diana", 8.8),
        Student(101, "Alice", 9.6),
        Student(103, "Charlie", 7.9),
        Student(102, "Bob", 9.1)
    };

    in_place_merge_sort_container(students);
    std::cout << "Students sorted by ID (default operator<):\n";
    for (const auto& s : students) {
        std::cout << "  ID: " << s.getId() << ", Name: " << s.getName() << ", GPA: " << s.getGpa() << "\n";
    }

    in_place_merge_sort_container(students, [](const Student& a, const Student& b) {
        return a.getGpa() > b.getGpa();
    });
    std::cout << "Students sorted by GPA (descending via lambda):\n";
    for (const auto& s : students) {
        std::cout << "  ID: " << s.getId() << ", Name: " << s.getName() << ", GPA: " << s.getGpa() << "\n";
    }

    std::cout << "\n=== DEMO 4: Bogosort ===\n";
    std::vector<int> smallVec = {15, 3, 9, 1};
    std::cout << "Original small vector: ";
    for (int n : smallVec) std::cout << n << " ";
    std::cout << "\n";

    bogo_sort_container(smallVec);
    std::cout << "Bogosort result: ";
    for (int n : smallVec) std::cout << n << " ";
    std::cout << "\n";
}

void printMenu() {
    std::cout << "\n============================================\n";
    std::cout << "   LAB 4: Generic Programming & Wirth Graph \n";
    std::cout << "============================================\n";
    std::cout << "1. Run Sorting Algorithms Demonstrations\n";
    std::cout << "2. Display current Graph\n";
    std::cout << "3. Add Vertex to Graph\n";
    std::cout << "4. Add Edge to Graph\n";
    std::cout << "5. Remove Vertex from Graph\n";
    std::cout << "6. Remove Edge from Graph\n";
    std::cout << "7. Check Vertex / Edge existence\n";
    std::cout << "8. Inspect Vertex Degree & Edge Degree\n";
    std::cout << "9. Traverse Vertices (Forward & Reverse iterators)\n";
    std::cout << "10. Traverse Edges (Edge iterators)\n";
    std::cout << "11. Traverse Adjacent Vertices (Bidirectional & Forward)\n";
    std::cout << "12. Traverse Incident Edges (Incident iterator)\n";
    std::cout << "13. Clear Graph\n";
    std::cout << "0. Exit\n";
    std::cout << "Select option: ";
}

int main() {
    Graph<std::string> graph = {"Minsk", "Brest", "Grodno", "Vitebsk"};
    graph.add_edge("Minsk", "Brest");
    graph.add_edge("Minsk", "Grodno");
    graph.add_edge("Minsk", "Vitebsk");
    graph.add_edge("Brest", "Grodno");

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            continue;
        }

        switch (choice) {
            case 1: {
                runSortDemos();
                break;
            }
            case 2: {
                std::cout << "\n--- Current Graph ---\n";
                std::cout << graph;
                break;
            }
            case 3: {
                std::string v;
                std::cout << "Enter vertex name: ";
                std::cin >> v;
                if (graph.add_vertex(v)) {
                    std::cout << "Vertex '" << v << "' added successfully.\n";
                } else {
                    std::cout << "Vertex '" << v << "' already exists!\n";
                }
                break;
            }
            case 4: {
                std::string u, v;
                std::cout << "Enter first vertex: ";
                std::cin >> u;
                std::cout << "Enter second vertex: ";
                std::cin >> v;
                try {
                    if (graph.add_edge(u, v)) {
                        std::cout << "Edge (" << u << " <-> " << v << ") added successfully.\n";
                    } else {
                        std::cout << "Edge already exists!\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 5: {
                std::string v;
                std::cout << "Enter vertex name to remove: ";
                std::cin >> v;
                if (graph.remove_vertex(v)) {
                    std::cout << "Vertex '" << v << "' removed.\n";
                } else {
                    std::cout << "Vertex '" << v << "' not found.\n";
                }
                break;
            }
            case 6: {
                std::string u, v;
                std::cout << "Enter first vertex: ";
                std::cin >> u;
                std::cout << "Enter second vertex: ";
                std::cin >> v;
                if (graph.remove_edge(u, v)) {
                    std::cout << "Edge removed.\n";
                } else {
                    std::cout << "Edge not found.\n";
                }
                break;
            }
            case 7: {
                std::string u, v;
                std::cout << "Check vertex: ";
                std::cin >> u;
                std::cout << "Vertex exists: " << (graph.has_vertex(u) ? "YES" : "NO") << "\n";
                std::cout << "Check edge to another vertex (enter second): ";
                std::cin >> v;
                std::cout << "Edge exists: " << (graph.has_edge(u, v) ? "YES" : "NO") << "\n";
                break;
            }
            case 8: {
                std::string v;
                std::cout << "Enter vertex name to inspect vertex degree: ";
                std::cin >> v;
                try {
                    std::cout << "Degree of vertex '" << v << "': " << graph.degree(v) << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }

                std::string u, w;
                std::cout << "Enter two vertices to inspect edge degree (u and v): ";
                std::cin >> u >> w;
                try {
                    std::cout << "Degree of edge (" << u << " <-> " << w << "): "
                              << graph.edge_degree(u, w) << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 9: {
                std::cout << "\nVertices via forward iterators: ";
                for (auto it = graph.cbegin(); it != graph.cend(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << "\nVertices via reverse iterators: ";
                for (auto it = graph.crbegin(); it != graph.crend(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << "\n";
                break;
            }
            case 10: {
                std::cout << "\nEdges via edge_iterator:\n";
                for (auto it = graph.edge_cbegin(); it != graph.edge_cend(); ++it) {
                    std::cout << "  (" << (*it).first << " <-> " << (*it).second << ")\n";
                }
                break;
            }
            case 11: {
                std::string v;
                std::cout << "Enter vertex name: ";
                std::cin >> v;
                try {
                    std::cout << "Adjacent vertices (bidirectional iterator): ";
                    for (auto it = graph.adjacent_vertex_begin(v); it != graph.adjacent_vertex_end(v); ++it) {
                        std::cout << *it << " ";
                    }
                    std::cout << "\nAdjacent vertices (Variant 11 forward iterator): ";
                    for (auto it = graph.forward_adjacent_begin(v); it != graph.forward_adjacent_end(v); ++it) {
                        std::cout << *it << " ";
                    }
                    std::cout << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 12: {
                std::string v;
                std::cout << "Enter vertex name: ";
                std::cin >> v;
                try {
                    std::cout << "Incident edges:\n";
                    for (auto it = graph.incident_edge_begin(v); it != graph.incident_edge_end(v); ++it) {
                        std::cout << "  (" << (*it).first << " <-> " << (*it).second << ")\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 13: {
                graph.clear();
                std::cout << "Graph cleared.\n";
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
                break;
        }
    }

    return 0;
}
