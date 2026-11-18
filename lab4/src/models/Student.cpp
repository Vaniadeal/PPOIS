#include "models/Student.hpp"
#include <utility>

namespace lab4 {

Student::Student() : id_(0), name_(""), gpa_(0.0) {}

Student::Student(int id, std::string name, double gpa)
    : id_(id), name_(std::move(name)), gpa_(gpa) {}

int Student::getId() const {
    return id_;
}

const std::string& Student::getName() const {
    return name_;
}

double Student::getGpa() const {
    return gpa_;
}

bool Student::operator<(const Student& other) const {
    return id_ < other.id_;
}

bool Student::operator==(const Student& other) const {
    return id_ == other.id_ && name_ == other.name_ && gpa_ == other.gpa_;
}

bool Student::operator!=(const Student& other) const {
    return !(*this == other);
}

} // namespace lab4
