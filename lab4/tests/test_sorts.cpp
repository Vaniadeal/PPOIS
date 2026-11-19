#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include "sorts.hpp"
#include "student.hpp"

using namespace lab4;

SUITE(SortsTests) {

TEST(IsSortedCustom_VariousScenarios) {
    std::vector<int> empty;
    CHECK(is_sorted_custom(empty.begin(), empty.end()));

    std::vector<int> single = {42};
    CHECK(is_sorted_custom(single.begin(), single.end()));

    std::vector<int> sorted = {1, 2, 2, 5, 9};
    CHECK(is_sorted_custom(sorted.begin(), sorted.end()));

    std::vector<int> unsorted = {1, 3, 2, 5};
    CHECK(!is_sorted_custom(unsorted.begin(), unsorted.end()));

    std::vector<int> desc = {9, 5, 2, 1};
    CHECK(!is_sorted_custom(desc.begin(), desc.end()));
    CHECK(is_sorted_custom(desc.begin(), desc.end(), std::greater<int>()));
}

TEST(BogoSort_VectorAndArray) {
    std::vector<int> empty;
    bogo_sort(empty.begin(), empty.end());
    CHECK(empty.empty());

    std::vector<int> single = {10};
    bogo_sort(single.begin(), single.end());
    CHECK_EQUAL(10, single[0]);

    std::vector<int> vec = {4, 1, 3, 2};
    bogo_sort(vec.begin(), vec.end());
    CHECK(is_sorted_custom(vec.begin(), vec.end()));
    CHECK_EQUAL(1, vec[0]);
    CHECK_EQUAL(2, vec[1]);
    CHECK_EQUAL(3, vec[2]);
    CHECK_EQUAL(4, vec[3]);

    int arr[] = {3, 1, 2};
    bogo_sort_container(arr);
    CHECK(is_sorted_custom(std::begin(arr), std::end(arr)));
    CHECK_EQUAL(1, arr[0]);
    CHECK_EQUAL(2, arr[1]);
    CHECK_EQUAL(3, arr[2]);

    std::vector<int> desc = {2, 4, 1};
    bogo_sort_container(desc, std::greater<int>());
    CHECK(is_sorted_custom(desc.begin(), desc.end(), std::greater<int>()));
    CHECK_EQUAL(4, desc[0]);
    CHECK_EQUAL(2, desc[1]);
    CHECK_EQUAL(1, desc[2]);
}

TEST(InPlaceMergeSort_BasicTypes) {
    std::vector<int> empty;
    in_place_merge_sort_container(empty);
    CHECK(empty.empty());

    std::vector<int> single = {100};
    in_place_merge_sort_container(single);
    CHECK_EQUAL(100, single[0]);

    std::vector<int> alreadySorted = {1, 2, 3, 4, 5};
    in_place_merge_sort_container(alreadySorted);
    CHECK(is_sorted_custom(alreadySorted.begin(), alreadySorted.end()));

    std::vector<int> reversed = {5, 4, 3, 2, 1};
    in_place_merge_sort_container(reversed);
    CHECK(is_sorted_custom(reversed.begin(), reversed.end()));
    CHECK_EQUAL(1, reversed[0]);
    CHECK_EQUAL(5, reversed[4]);

    std::vector<int> duplicates = {4, 2, 5, 2, 3, 4, 1};
    in_place_merge_sort_container(duplicates);
    CHECK(is_sorted_custom(duplicates.begin(), duplicates.end()));
    CHECK_EQUAL(1, duplicates[0]);
    CHECK_EQUAL(2, duplicates[1]);
    CHECK_EQUAL(2, duplicates[2]);
    CHECK_EQUAL(5, duplicates[6]);
}

TEST(InPlaceMergeSort_RawArray) {
    int arr[8] = {12, 3, 17, 8, 34, 1, 23, 9};
    in_place_merge_sort_container(arr);
    CHECK(is_sorted_custom(std::begin(arr), std::end(arr)));
    CHECK_EQUAL(1, arr[0]);
    CHECK_EQUAL(3, arr[1]);
    CHECK_EQUAL(8, arr[2]);
    CHECK_EQUAL(34, arr[7]);
}

TEST(InPlaceMergeSort_CustomComparator) {
    std::vector<std::string> words = {"banana", "apple", "pear", "fig"};
    in_place_merge_sort_container(words, std::greater<std::string>());
    CHECK_EQUAL("pear", words[0]);
    CHECK_EQUAL("fig", words[1]);
    CHECK_EQUAL("banana", words[2]);
    CHECK_EQUAL("apple", words[3]);
}

TEST(InPlaceMergeSort_CustomUserClassStudent) {
    std::vector<Student> students = {
        Student(103, "Alice", 8.5),
        Student(101, "Bob", 9.2),
        Student(105, "Charlie", 7.8),
        Student(102, "David", 9.5),
        Student(104, "Eve", 8.9)
    };

    in_place_merge_sort_container(students);
    CHECK(is_sorted_custom(students.begin(), students.end()));
    CHECK_EQUAL(101, students[0].getId());
    CHECK_EQUAL(102, students[1].getId());
    CHECK_EQUAL(103, students[2].getId());
    CHECK_EQUAL(104, students[3].getId());
    CHECK_EQUAL(105, students[4].getId());

    auto byGpaDesc = [](const Student& a, const Student& b) {
        return a.getGpa() > b.getGpa();
    };

    in_place_merge_sort_container(students, byGpaDesc);
    CHECK_EQUAL("David", students[0].getName());
    CHECK_EQUAL("Bob", students[1].getName());
    CHECK_EQUAL("Eve", students[2].getName());
    CHECK_EQUAL("Alice", students[3].getName());
    CHECK_EQUAL("Charlie", students[4].getName());
}

}

int main() {
    return UnitTest::RunAllTests();
}
