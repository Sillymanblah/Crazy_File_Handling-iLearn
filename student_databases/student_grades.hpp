#pragma once
#ifndef STUDENT_GRADES_HPP
#define STUDENT_GRADES_HPP

#include <iostream>
#include <string>

template <class _Student, class _Grades>
struct student_grades
{
    using student_type = _Student;
    using grades_set = _Grades;

    _Student student_data;
    _Grades grades;

    static std::vector<std::string> determine_headers();

    void print(std::ostream& output, char delim = ',') const
    {
        student_data.print(output, delim);
        grades.print(output, delim);
    }
    void read(std::istream& input, char delim = ',')
    {
        student_data.read(input, delim);
        grades.read(input, delim);
    }
};

#endif // STUDENT_GRADES_HPP