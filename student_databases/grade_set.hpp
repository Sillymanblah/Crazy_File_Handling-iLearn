#pragma once
#ifndef GRADE_SET_HPP
#define GRADE_SET_HPP

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// grade_type must have a `static constexpr const char* description` value
template <class _Gty, size_t _Size>
struct grade_set : public std::array<_Gty, _Size>
{
    using _Mybase           =   std::array<_Gty, _Size>;
    using size_type         =   size_t;
    using difference_type   =   ptrdiff_t;
    using grade_type        =   typename _Mybase::value_type;
    using pointer           =   typename _Mybase::pointer;
    using const_pointer     =   typename _Mybase::const_pointer;
    using reference         =   typename _Mybase::reference;
    using const_reference   =   typename _Mybase::const_reference;

    using iterator                  =   typename _Mybase::iterator;
    using const_iterator            =   typename _Mybase::const_iterator;
    using reverse_iterator          =   typename _Mybase::reverse_iterator;
    using const_reverse_iterator    =   typename _Mybase::const_reverse_iterator;

    static std::vector<std::string> determine_headers()
    {
        std::vector<std::string> result;
        std::string user_input;
        size_t counter = 1, count = 1;
        bool all_same = false;

        for (size_type index = 0; index < _Size; ++index)
        {
            std::string input;
            if (counter == count)
            {
                std::cout << "What is the title of the grade?\n";
                std::getline(std::cin, input);
                std::cout << "You have selected \"" << input << "\", ";
                std::cout << "how many grades should share this format?";
                std::cin >> count;
                counter = 0;
            }
            if (counter > 1 && counter < count) user_input = input + ' ' + std::to_string(++counter);
            else user_input = input;
            result.push_back(user_input + ' ' + grade_type::description);
        }
        return result;
    }
    
    void print(std::ostream& output, char delim = ',') const
    { for (const_iterator iter = this->cbegin(); iter != this->cend(); ++iter) iter->print(output, delim); }
    void read(std::istream& input, char delim = ',')
    { for (iterator iter = this->begin(); iter != this->end(); ++iter) iter->read(input, delim); }
};

template <class _Ty, _Ty _Max>
struct number_grade
{
    static constexpr const char* description = "Points Grade";

    static_assert(std::is_integral_v<_Ty>, "The struct \"number_grade\" requires the type to be an integral type.");
    
    using value_type        =   _Ty;
    using rvalue            =   _Ty&&;
    using reference         =   _Ty&;
    using const_reference   =   const _Ty&;
    using pointer           =   _Ty*;
    using const_pointer     =   const _Ty*;

    value_type value;

    void max()
    { value = _Max; }
    void limit()
    { if (value > _Max) max(); }

    number_grade() = default;
    number_grade(const_reference _value) : value(_value) {}
    number_grade(rvalue _value) : value(_value) {}
    number_grade& operator =(const_reference _value) { this->value = _value; return *this; }
    number_grade& operator =(rvalue _value) { this->value = _value; return *this; }
    operator value_type() { return value; }

    void print(std::ostream& output, char delim = ',') const
    { output << (size_t)value << delim; }
    void read(std::istream& input, char delim = ',')
    {
        std::string temp;
        std::getline(input, temp, delim);
        std::stringstream(temp) >> value;
    }
};

struct letter_grade
{
    static constexpr const char* description = "Letter Grade";

    using value_type        =   char;
    using rvalue            =   char&&;
    using reference         =   char&;
    using const_reference   =   const char&;
    using pointer           =   char*;
    using const_pointer     =   const char*;

    value_type value;

    letter_grade() = default;
    letter_grade(const_reference _value) : value(_value) {}
    letter_grade(rvalue _value) : value(_value) {}
    letter_grade& operator =(const_reference _value) { this->value = _value; return *this; }
    letter_grade& operator =(rvalue _value) { this->value = _value; return *this; }
    operator value_type() { return value; }

    void print(std::ostream& output, char delim = ',') const
    { output << value << delim; }
    void read(std::istream& input, char delim = ',')
    {
        std::string temp;
        std::getline(input, temp, delim);
        value = temp[0];
    }
};

struct written_grade
{
    static constexpr const char* description = "Written Grade";
    
    using value_type        =   std::string;
    using rvalue            =   std::string&&;
    using reference         =   std::string&;
    using const_reference   =   const std::string&;
    using pointer           =   std::string*;
    using const_pointer     =   const std::string*;

    value_type value;

    written_grade() = default;
    written_grade(const_reference _value) : value(_value) {}
    written_grade(rvalue _value) : value(_value) {}
    written_grade& operator =(const_reference _value) { this->value = _value; return *this; }
    written_grade& operator =(rvalue _value) { this->value = _value; return *this; }
    operator value_type() { return value; }

    void print(std::ostream& output, char delim = ',') const
    { output << value << delim; }
    void read(std::istream& input, char delim = ',')
    { std::getline(input, value, delim); }
};

#endif // GRADE_SET_HPP