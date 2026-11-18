#pragma once

#include <string>

namespace lab4 {

class Student {
private:
    int id_;
    std::string name_;
    double gpa_;

public:
    Student();
    Student(int id, std::string name, double gpa);

    int getId() const;
    const std::string& getName() const;
    double getGpa() const;

    bool operator<(const Student& other) const;
    bool operator==(const Student& other) const;
    bool operator!=(const Student& other) const;
};

} // namespace lab4
