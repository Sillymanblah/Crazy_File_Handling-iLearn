#pragma once
#ifndef DRIVER_HELPER_HPP
#define DRIVER_HELPER_HPP

#include "./database/database.hpp"
#include "./student_databases/student.hpp"

#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

// Functions are below are for driver case handles.

enum class USAGE
{
    argc = 1,
    bad_folder = 3,
    bad_html_file = 5,
    bad_csv_file = 6,
    open_error = 10,
};

[[noreturn]] void usage( const char* _executable, USAGE _error )
{
    std::cerr << "Invalid call to \"" << _executable << "\n";
    std::cerr << "The program expects 5 arguments:\n";
    std::cerr << "The executable, a folder path to change the directory to,\n";
    std::cerr << "an HTML file for it to read, a CSV file to send submissions to,\n";
    std::cerr << "and a csv file to send the grade database to.\n\n";

    if (_error == USAGE::argc) std::cerr << "You did not send in the proper number of arguments.\n";
    if (_error == USAGE::bad_folder) std::cerr << "The folder that was sent to the file does not appear to exist.\n";
    if (_error == USAGE::bad_html_file) std::cerr << "The first file you sent in does not appear to be an HTML.\n";
    if (_error == USAGE::bad_csv_file) std::cerr << "The second file you sent in does not appear to be a CSV.\n";
    if (_error == USAGE::open_error) std::cerr << "That file could not be opened, double check it exists and try again.\n";
    exit(EXIT_FAILURE);
}

bool check_filename( std::string _filename, std::string _extension )
{ 
    size_t extension = _filename.rfind('.');

    if (extension == _filename.npos) return false;
    if (_filename.substr(extension) != _extension) return false;

    return true;
}

void validate( int argc, const char** argv )
{
    if (argc != 5) throw USAGE::argc;
    if (!check_filename(argv[2], ".html")) throw USAGE::bad_html_file;
    if (!check_filename(argv[3], ".csv")) throw USAGE::bad_csv_file;
    if (!check_filename(argv[4], ".csv")) throw USAGE::bad_csv_file;
    std::cout << "Validated!";
}

void create_file(const char* filename)
{
    std::ofstream file(filename);
}

void open_file(std::fstream& to_open, const char* filename)
{
    to_open.open(filename);
    if (!to_open.is_open()) throw USAGE::open_error;
}

void open_files(size_t count, std::fstream* files, const char** filenames)
{
    for (size_t index = 0; index < count; ++index)
        open_file(files[index], filenames[index]);
}

void print_errors(std::exception& exc, std::ostream& output = std::cerr, size_t indent = 0)
{
    output << std::string(' ', indent) << exc.what();

    try
    {
        std::rethrow_if_nested(exc);
    }
    catch(const std::exception& esc)
    {
        print_errors(exc, output, indent+1);
    }
}

unsigned long get_id(const std::string& id_submission)
{
    unsigned long id;
    std::stringstream(id_submission) >> id;
    return id;
}

void remove_non_alpha(std::string& str)
{
    for ( std::string::iterator iter = str.begin(); iter < str.end(); ++iter )
        if (!isalpha(*iter)) iter = --str.erase(iter);
}
void lowercase(std::string& str)
{
    for ( char& element : str)
        element = tolower(element);
}
void limit_size(std::string& str, uint16_t size)
{
    if (str.size() > size)
        str.resize(size);
}

using student_id                =   unsigned long;
using student_database          =   DB::database<student_id, student>;
using student_email_database    =   DB::database<student_username, student_email>;

void port_data(student& new_db_entry, const student_email_database& old_db, student_name _student_name)
{
    std::string key = _student_name.last_name;
    remove_non_alpha(key); lowercase(key); limit_size(key, 8);

    for (auto data_set : old_db.data_base)
        if (data_set.first.username.find(key) != std::string::npos)
        {
            new_db_entry = student(_student_name.first_name, _student_name.last_name, data_set.first.username, data_set.second.email);
            return;
        }
}

#endif // DRIVER_HELPER_HPP