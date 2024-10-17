#pragma once
#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <iostream>

struct student_username
{
    static constexpr const char* description[] = {"Username"};

    std::string username;
    
    void print(std::ostream& output, char delim = ',') const
    { output << username << delim; }
    void read(std::istream& input, char delim = ',')
    { std::getline(input, username, delim); }

    friend std::ostream& operator <<(std::ostream& output, const student_username& obj)
    {
        output << obj.username;
        return output;
    }
    friend std::istream& operator >>(std::istream& input, student_username& obj)
    {
        std::getline(input, obj.username);
        return input;
    }

    bool operator ==(const student_username& _other) const
    { return this->username == _other.username; }
    bool operator !=(const student_username& _other) const
    { return this->username != _other.username; }
    bool operator <(const student_username& _other) const
    { return this->username < _other.username; }
    bool operator >(const student_username& _other) const
    { return this->username > _other.username; }
    bool operator <=(const student_username& _other) const
    { return this->username <= _other.username; }
    bool operator >=(const student_username& _other) const
    { return this->username >= _other.username; }
};

struct student_name
{
    static constexpr const char* description[] = {"Last Name", "First Name"};

    std::string first_name, last_name;
    
    void print(std::ostream& output, char delim = ',') const
    { output << last_name << delim << first_name << delim; }
    void read(std::istream& input, char delim = ',')
    {
        std::getline(input >> std::ws, last_name, delim);
        std::getline(input >> std::ws, first_name, delim);
    }

    bool operator ==(const student_name& _other) const
    { return this->last_name == _other.last_name && this->first_name == _other.first_name; }
    bool operator !=(const student_name& _other) const
    { return !(*this == _other); }
    bool operator <(const student_name& _other) const
    {
        return  this->last_name < _other.last_name ||
                this->last_name == _other.last_name && this->first_name < _other.first_name;
    }
    bool operator >(const student_name& _other) const
    { return _other < *this; }
    bool operator <=(const student_name& _other) const
    { return !(*this > _other); }
    bool operator >=(const student_name& _other) const
    { return !(*this < _other); }
};

struct student_email
{
    static constexpr const char* description[] = {"Email"};

    std::string email;
    
    void print(std::ostream& output, char delim = ',') const
    { output << email << delim; }
    void read(std::istream& input, char delim = ',')
    { std::getline(input, email, delim); }

    bool operator ==(const student_email& _other) const
    { return this->email == _other.email; }
    bool operator !=(const student_email& _other) const
    { return this->email != _other.email; }
    bool operator <(const student_email& _other) const
    { return this->email < _other.email; }
    bool operator >(const student_email& _other) const
    { return this->email > _other.email; }
    bool operator <=(const student_email& _other) const
    { return this->email <= _other.email; }
    bool operator >=(const student_email& _other) const
    { return this->email >= _other.email; }
};

struct student : public student_name, public student_username, public student_email
{
    static constexpr const char* description[] = {"Username", "Last Name", "First Name", "Email"};

    student() = default;
    student
    (
        std::string _first_name,
        std::string _last_name,
        std::string _username,
        std::string _email
    ) :
    student_name{_first_name, _last_name},
    student_username{_username},
    student_email{_email}
    {}
    student( const student& _student ) = default;
    student( student&& _student ) = default;
    student& operator =( const student& _student ) = default;
    student& operator =( student&& _student ) = default;

    void print(std::ostream& output, char delim = ',') const
    {
        student_username::print(output, delim);
        student_name::print(output, delim);
        student_email::print(output, delim);
    }
    void read(std::istream& input, char delim = ',')
    {
        student_username::read(input, delim);
        student_name::read(input, delim);
        student_email::read(input, delim);
    }

    bool operator ==(const student& _other) const
    { return (student_name)*this == (student_name)_other; }
    bool operator !=(const student& _other) const
    { return (student_name)*this != (student_name)_other; }
    bool operator <(const student& _other) const
    { return (student_name)*this < (student_name)_other; }
    bool operator >(const student& _other) const
    { return (student_name)*this > (student_name)_other; }
    bool operator <=(const student& _other) const
    { return (student_name)*this <= (student_name)_other; }
    bool operator >=(const student& _other) const
    { return (student_name)*this >= (student_name)_other; }
};

#endif // STUDENT_HPP