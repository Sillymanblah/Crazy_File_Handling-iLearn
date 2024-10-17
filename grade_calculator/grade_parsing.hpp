#ifndef PARSE_GRADED_HPP
#define PARSE_GRADED_HPP

#include "../functional_parsing/functional_parsing.hpp"
#include "../student_databases/grade_set.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

using directory_contents = std::vector<std::filesystem::path>;

directory_contents find_files(const std::string& student)
{
    std::filesystem::path root = std::filesystem::current_path().append(student);
    directory_contents files;

    for (const std::filesystem::directory_entry& sub_directory : std::filesystem::recursive_directory_iterator{root})
    {
        if (std::filesystem::is_regular_file(sub_directory)) files.push_back(sub_directory.path());
    }

    return files;
}

bool always_true(const std::string&)
{ return true; }
bool always_false(const std::string&)
{ return false; }

bool has_comment(const std::string& line)
{
    return line.find("//") != std::string::npos;
}

template <class _Ty, _Ty _Max>
void grade_student(const std::string& student, number_grade<_Ty, _Max>& grade)
{
    grade.max();

    directory_contents files = find_files(student);
    std::ifstream input;

    sreader break_all = [](std::istream& input, std::string& output)
    {
        input >> std::ws >> output;
        if (output.size() != 0) output += ' ';
        return (bool)input;
    };

    sparser split_all = build_parser<char> (
        always_true,
        always_true,
        break_all
    ),
    isolate_comments = build_parser<char> (
        build_string_conditional<char>("// "),
        always_false,
        break_all
    ),
    get_ta_comments = build_parser<char> (
        build_string_conditional<char>("// TA:"),
        always_false,
        [](std::istream& input, std::string& output)
        {
            if (!std::getline(input, output, ':').eof()) output += ':';
            return (bool)input;
        }
    );

    for (const std::filesystem::path& file : files)
    {
        input.open(file.string());

        parsed_string lines;
        std::string line, working;
        
        while (std::getline(input, line))
        {
            std::stringstream buffer(line);
            parsed_string temp = isolate_comments(buffer);

            if (temp.size() == 0) continue;

            working = temp[0];
            buffer = std::stringstream(working);
            temp = get_ta_comments(buffer);

            if (temp.size() == 0) continue;

            working = temp[0];
            
            buffer = std::stringstream(working);
            temp = split_all(buffer);

            if (temp.size() == 0) continue;

            try
            {
                int adjustment = std::stoi(temp.back());
                grade = grade + adjustment;
            }
            catch(...) {}
        }
    }
    grade.limit();
}

#endif // PARSE_GRADED_HPP