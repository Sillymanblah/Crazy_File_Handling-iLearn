#include "./UTILS/ANSI_COLOR.hpp"
#include "./driver_helper.hpp"
#include "./html_parser/html_parsing.hpp"
#include "./student_databases/student.hpp"
#include "./database/database.hpp"
#include "./student_databases/student_submissions.hpp"
#include "./student_databases/student_grades.hpp"
#include "./student_databases/grade_set.hpp"
#include "./grade_calculator/grade_parsing.hpp"

#include <filesystem>

using student_id                =   unsigned long;
using submission_database       =   DB::database<student_id, student_submissions>;

using lab_grade     =   number_grade<unsigned char, 100>;
using lab_grades    =   DB::database<student_username, lab_grade>;

template<>
header_set lab_grades::determine_headers_manually()
{
    header_set result; std::string input;

    result.push_back("Username");
    std::cout << "Please input the name of the lab:\n";
    std::getline(std::cin, input);
    result.push_back(input + ' ' + lab_grade::description);
    result.push_back("End-of-Line Indicator");

    return result;
}
template<>
header_set student_database::determine_headers_manually()
{
    header_set result; std::string input;

    result.push_back("OrgDefinedId");
    for (int i = 0; i < 4; ++i)
        result.push_back(student::description[i]);
    result.push_back("End-of-Line Indicator");

    return result;
}
template<>
header_set student_email_database::determine_headers_manually()
{
    header_set result; std::string input;

    result.push_back("OrgDefinedId");
    result.push_back(student_email::description[0]);
    result.push_back("End-of-Line Indicator");

    return result;
}

int main(const int argc, const char** argv)
{
    std::filesystem::path curr = "C:\\Users\\John\\OneDrive\\Documents\\School\\3rd Year - Spring\\CSC 1310 - TA Job\\Working\\temp\\grading";
    std::string deadline;

    std::cout << "What was the submission deadline? (Mon [day], [year] [hours][minute] AM/PM)\n";
    std::getline(std::cin, deadline);

    std::fstream html_file;
    submission_database _submission_database;
    lab_grades graded_labs;

    student_email_database older;
    student_database newer;

    older.data_file.open(curr / "students.csv");
    older.determine_headers(true);
    older.read_data();

    std::ofstream(curr / "students_new.csv");
    newer.data_file.open(curr / "students_new.csv");
    newer.determine_headers(false);

    try
    {
        validate(argc, argv);

        std::filesystem::current_path(std::filesystem::path(argv[1]));

        open_file(html_file, argv[2]);
        create_file(argv[3]);
        create_file(argv[4]);
        open_file(_submission_database.data_file, argv[3]);
        open_file(graded_labs.data_file, argv[4]);

        std::cout << "Building the submission database header:\n";
        _submission_database.determine_headers(false);
        std::cout << "Building the graded lab database header:\n";
        graded_labs.determine_headers(false);

        parsed_data students_and_submissions = do_read(html_file);

        for (auto student_submissions : students_and_submissions)
        {
            student_id id = get_id(student_submissions.second[0][0]);
            lab_grade current;

            _submission_database.data_base[id] = student_submissions;
            student_name name = _submission_database.data_base[id].get_student_name();

            std::string this_student = name.last_name + ", " + name.first_name;
            
            port_data(newer.data_base[id], older, name);

            grade_student(this_student, current);

            graded_labs.data_base[newer.data_base[id]] = current;

            _submission_database.data_base[id].print_with_colors (
                std::cout,
                ilearn_time::get_minutes(deadline),
                get_basic_color(ANSI::COLOR::BASIC::YELLOW),
                get_basic_color(ANSI::COLOR::BASIC::GREEN),
                get_basic_color(ANSI::COLOR::BASIC::RED),
                get_basic_color(ANSI::COLOR::BASIC::CYAN),
                get_basic_color(ANSI::COLOR::BASIC::BLUE)
            );
        }

        _submission_database.print(';');
        graded_labs.print();
        newer.print();
    }
    catch(USAGE error)
    {
        usage(argv[0], error);
    }
    catch(std::exception& exc)
    {
        print_errors(exc);
    }
}