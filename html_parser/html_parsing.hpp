#ifndef HTML_PARSING_HPP
#define HTML_PARSING_HPP

#include "../functional_parsing/functional_parsing.hpp"

#include <map>
#include <sstream>

// Functions below are for use in parsing the actual file.

template <class _Elem>
bool get_html_block(std::basic_istream<_Elem>& input, std::basic_string<_Elem>& output)
{
    input >> std::ws;
    if (input.peek() == '<')
    {
        if (!std::getline(input.ignore(), output, '>'))
            return false;
        output = '<' + output + '>';
        return true;
    }
    if (!std::getline(input, output, '<'))
        return false;
    input.putback('<');
    return true;
}

template <class _Elem>
bool is_command(const std::basic_string<_Elem>& str)
{ return str.front() == (_Elem)'<' && str.back() == (_Elem)'>'; }

template <class _Elem>
bool is_not_command(const std::basic_string<_Elem>& str)
{ return !is_command(str); }

// The below functions for performing the parsing and collecting the data.

template <class _Elem>
using basic_submission_set          =   std::vector<basic_parsed_string<_Elem>>;
template <class _Elem>
using basic_student_submission_set  =   std::map<std::basic_string<_Elem>, basic_submission_set<_Elem>>;

template <class _Elem>
std::basic_string<_Elem> parse_table(std::basic_istream<_Elem>& input)
{
    basic_parsed_string<_Elem> tables = manual_parse<_Elem> (
        input,
        build_string_conditional<_Elem>("<table WIDTH=100% cellpadding=3 cellspacing=0 border=0 bgcolor=white>"),
        build_string_conditional<_Elem>("</table>"),
        get_html_block<_Elem>
    );

    if (tables.size() == 0) throw std::runtime_error("No table was found in the html file!");
    if (tables.size() > 1) throw std::runtime_error("Got more tables than expected.");

    return tables[0];
}

template <class _Elem>
basic_parsed_string<_Elem> parse_non_commands(const std::basic_string<_Elem>& input)
{
    std::basic_stringstream<_Elem> buffer(input);
    basic_parsed_string<_Elem> non_commands = manual_parse<_Elem> (
        buffer,
        std::function(is_not_command<_Elem>),
        std::function(is_command<_Elem>),
        get_html_block<_Elem>
    );
    if (non_commands.size() == 0) throw std::runtime_error("Got zero non-commands, expected at least one...");
    return non_commands;
}

template <class _Elem>
basic_submission_set<_Elem> parse_submissions(const std::basic_string<_Elem>& input)
{
    std::basic_stringstream<_Elem> buffer(input);
    basic_submission_set<_Elem> submissions;

    static const basic_parser<_Elem> find_submissions = build_parser<_Elem> (
        build_string_conditional<_Elem>("<tr bgcolor=white>"),
        build_string_conditional<_Elem>("</tr>"),
        get_html_block<_Elem>
    );

    for (const std::basic_string<_Elem>& submission : find_submissions(buffer))
    {        
        basic_parsed_string<_Elem> submission_info = parse_non_commands<_Elem>(submission);
        if (submission_info.size() == 0) throw std::runtime_error("Submission with no data found...");

        submissions.push_back(submission_info);
    }
    if (submissions.size() == 0) throw std::runtime_error("Failed to find any submissions for the student...");

    return submissions;
}

template <class _Elem>
void parse_student_submissions(basic_student_submission_set<_Elem>& student_submissions, const std::basic_string<_Elem>& student_with_submissions)
{
    std::basic_string<_Elem> student;

    basic_parsed_string<_Elem> student_information = parse_non_commands<_Elem>(student_with_submissions);

    student = student_information[0];
    try
    {
        basic_submission_set<_Elem> submissions = parse_submissions<_Elem>(student_with_submissions);

        student_submissions[student] = submissions;
    }
    catch(const std::exception& e)
    {
        std::throw_with_nested(std::runtime_error(("Failed to get submissions for student " + student).c_str()));
    }
}

template <class _Elem>
basic_student_submission_set<_Elem> do_read(std::basic_iostream<_Elem>& input)
{
    basic_student_submission_set<_Elem> student_submissions;
    basic_string_conditional<_Elem> student_seperator = build_string_conditional<_Elem>("<tr bgcolor=#AAAAAA>");
    std::basic_stringstream<_Elem> buffer;

    basic_parser<_Elem> find_students = build_parser<_Elem> (
        student_seperator,
        student_seperator,
        get_html_block<_Elem>
    );

    std::basic_string<_Elem> table = parse_table<_Elem>(input);

    buffer = std::basic_stringstream<_Elem>(table);
    basic_parsed_string<_Elem> students = find_students(buffer);
    
    for (size_t index = 0; index < students.size(); ++index)
    {
        parse_student_submissions<_Elem>(student_submissions, students[index]);
    }

    return student_submissions;
}

#endif // HTML_PARSING_HPP