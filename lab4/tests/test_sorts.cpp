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

    int rawSingle[] = {100};
    CHECK(is_sorted_custom(std::begin(rawSingle), std::end(rawSingle)));
    int rawUnsorted[] = {2, 1};
    CHECK(!is_sorted_custom(std::begin(rawUnsorted), std::end(rawUnsorted)));
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

    int singleArr[] = {3};
    bogo_sort_container(singleArr);
    CHECK_EQUAL(3, singleArr[0]);

    std::vector<int> emptyDesc;
    bogo_sort_container(emptyDesc, std::greater<int>());
    CHECK(emptyDesc.empty());

    std::vector<int> singleDesc = {5};
    bogo_sort_container(singleDesc, std::greater<int>());
    CHECK_EQUAL(5, singleDesc[0]);

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

TEST(InPlaceMergeCustom_DirectInvocationsAndEdgeCases) {
    std::vector<int> v = {1, 5, 2, 4};
    in_place_merge_custom(v.begin(), v.begin() + 2, v.end(), std::less<int>());
    CHECK_EQUAL(1, v[0]);
    CHECK_EQUAL(2, v[1]);
    CHECK_EQUAL(4, v[2]);
    CHECK_EQUAL(5, v[3]);

    in_place_merge_custom(v.begin(), v.begin(), v.end(), std::less<int>());
    in_place_merge_custom(v.begin(), v.end(), v.end(), std::less<int>());
    CHECK_EQUAL(1, v[0]);

    std::vector<int> v2 = {3, 1};
    in_place_merge_custom(v2.begin(), v2.begin() + 1, v2.end(), std::less<int>());
    CHECK_EQUAL(1, v2[0]);
    CHECK_EQUAL(3, v2[1]);
}

TEST(InPlaceMergeSort_RawArray) {
    int singleArr[1] = {99};
    in_place_merge_sort_container(singleArr);
    CHECK_EQUAL(99, singleArr[0]);

    int arr[8] = {12, 3, 17, 8, 34, 1, 23, 9};
    in_place_merge_sort_container(arr);
    CHECK(is_sorted_custom(std::begin(arr), std::end(arr)));
    CHECK_EQUAL(1, arr[0]);
    CHECK_EQUAL(3, arr[1]);
    CHECK_EQUAL(8, arr[2]);
    CHECK_EQUAL(34, arr[7]);
}

TEST(InPlaceMergeSort_CustomComparator) {
    std::vector<std::string> emptyWords;
    in_place_merge_sort_container(emptyWords, std::greater<std::string>());
    CHECK(emptyWords.empty());

    std::vector<std::string> singleWord = {"single"};
    in_place_merge_sort_container(singleWord, std::greater<std::string>());
    CHECK_EQUAL("single", singleWord[0]);

    std::vector<std::string> words = {"banana", "apple", "pear", "fig"};
    in_place_merge_sort_container(words, std::greater<std::string>());
    CHECK_EQUAL("pear", words[0]);
    CHECK_EQUAL("fig", words[1]);
    CHECK_EQUAL("banana", words[2]);
    CHECK_EQUAL("apple", words[3]);
}

TEST(InPlaceMergeSort_CustomUserClassStudent) {
    Student defaultStudent;
    CHECK_EQUAL(0, defaultStudent.getId());
    CHECK_EQUAL("", defaultStudent.getName());
    CHECK_EQUAL(0.0, defaultStudent.getGpa());

    std::vector<Student> emptyStudents;
    in_place_merge_sort_container(emptyStudents);
    CHECK(emptyStudents.empty());

    std::vector<Student> singleStudent = {Student(1, "Only", 4.0)};
    in_place_merge_sort_container(singleStudent);
    CHECK_EQUAL(1, singleStudent[0].getId());

    std::vector<Student> students = {
        Student(103, "Alice", 8.5),
        Student(101, "Bob", 9.2),
        Student(105, "Charlie", 7.8),
        Student(102, "David", 9.5),
        Student(104, "Eve", 8.9)
    };

    CHECK(students[0] != students[1]);

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
