#ifndef STUDENT_SUBMISSIONS_HPP
#define STUDENT_SUBMISSIONS_HPP

#include "../UTILS/ilearn_time.hpp"
#include "../UTILS/ANSI.hpp"
#include "./student.hpp"
#include "../html_parser/html_parsing.hpp"

#include <ctime>
#include <map>
#include <string>
#include <vector>

using parsed_data       =   basic_student_submission_set<char>;
using parsed_data_index =   typename parsed_data::value_type;

struct submission
{
    std::string title;
    std::vector<std::string> comments;
    uint32_t time; // Representing in minutes since 0 AD

    void handle_parsed_data(const parsed_string& _data)
    {
        if (_data.size() > 1) title = _data[0];

        bool is_comment = false;
        for (size_t index = 1; index < _data.size(); ++index)
        {
            if (_data[index] == "Submitted:")
            {
                is_comment = false;
                time = ilearn_time::get_minutes(_data[++index]);
            }
            if (is_comment) comments.push_back(_data[index]);
            if (_data[index] == "Comments:") is_comment = true;
        }
    }

    submission() = default;
    submission( const submission& ) = default;
    submission( submission&& ) = default;
    submission( const parsed_string& _data )
    { handle_parsed_data( _data ); }

    submission& operator =( const submission& ) = default;
    submission& operator =( submission&& ) = default;
    submission& operator =( const parsed_string& _data )
    { handle_parsed_data( _data ); return *this; }

    void print_with_colors (
        std::ostream& output,
        uint32_t deadline = -1,
        ANSI::COMMANDS on_time = ANSI::DEFAULT,
        ANSI::COMMANDS late = ANSI::DEFAULT,
        ANSI::COMMANDS comments = ANSI::DEFAULT,
        ANSI::COMMANDS date = ANSI::DEFAULT
    )
    {
        output << ( time < deadline ? on_time : late ) << title << std::endl;
        output << ANSI::RESET << "\tComments:\n" << comments;
        for ( std::string& comment : this->comments )
            output << "\t\t\"" << comment << "\"\n";
        if ( this->comments.size() == 0 ) output << "\t\tNone!\n";
        output << ANSI::RESET << "\tSubmitted:\n\t\t" << date << ilearn_time::get_date_time( time ) << '\n';
    }

    friend std::ostream& operator << ( std::ostream& output, submission data )
    {
        data.print_with_colors( output ); // Nothing in, prints black and white.
        return output;
    }

    void print(std::ostream& output, const char separator = ',') const
    { output << title << separator; }
    void read(std::istream& input, const char separator = ',')
    { std::getline(input, title, separator); }
};

using submissions           =   std::vector<submission>;

class student_submissions
{
    student_name _student;
    submissions _submissions;

public:
    student_submissions() = default;
    student_submissions(const student_submissions&) = default;
    student_submissions(student_submissions&&) = default;
    student_submissions(parsed_data_index _data)
    {
        std::stringstream buffer(_data.first);
        _student.read(buffer);

        for (parsed_string& _submission : _data.second)
            _submissions.push_back(_submission); // Submission class handles the conversion.
    }
    student_submissions& operator =(const student_submissions&) = default;
    student_submissions& operator =(student_submissions&&) = default;
    student_submissions& operator =(parsed_data_index _data)
    {
        std::stringstream buffer(_data.first);
        _student.read(buffer);

        _submissions.clear();
        for (parsed_string& _submission : _data.second)
            _submissions.push_back(_submission);

        return *this;
    }

    static std::vector<std::string> determine_headers()
    {
        std::vector<std::string> headers;
        headers.push_back(student_name::description[0]);
        headers.push_back(student_name::description[1]);
        headers.push_back("Submissions...");
        return headers;
    }

    void print(std::ostream& output, const char separator = ',') const
    {
        _student.print(output, separator);

        for (const submission& _submission : _submissions)
        {
            _submission.print(output, separator);
        }
    }
    void read(std::istream& input, const char separator = ',')
    {
        _student.read(input, separator);

        submission temp;
        _submissions.clear();

        while (!input.eof())
        {
            temp.read(input, separator);
            _submissions.push_back(temp);
        }
    }

    void print_with_colors (
        std::ostream& output,
        uint32_t deadline = -1,
        ANSI::COMMANDS student = ANSI::DEFAULT,
        ANSI::COMMANDS on_time = ANSI::DEFAULT,
        ANSI::COMMANDS late = ANSI::DEFAULT,
        ANSI::COMMANDS comments = ANSI::DEFAULT,
        ANSI::COMMANDS date = ANSI::DEFAULT
    )
    {
        output << student << _student.last_name << ',' << _student.first_name << ":\n" << ANSI::RESET;
        
        for (submission& _submission : _submissions)
            _submission.print_with_colors(output, deadline, on_time, late, comments, date);
        
        output << ANSI::RESET;
    }
    const student_name& get_student_name()
    { return _student; }
};

#endif // STUDENT_SUBMISSIONS_HPP