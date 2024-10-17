#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include <map>

typedef int point_grade;
typedef std::vector<point_grade> point_grades;
typedef std::string id_type;

struct student
{
    std::string username;
    point_grades grades;

    friend std::ostream& operator <<( std::ostream& os, student _student )
    {
        os << _student.username;
        for ( point_grade grade : _student.grades )
            os << "," << grade;
        return os;
    }
};

typedef std::pair<std::string, std::string> adjustment;
typedef std::vector<adjustment> adjusments;
typedef std::vector<std::string> string_set;
typedef std::vector<student> class_data;
typedef std::pair<string_set, class_data> class_database;
typedef std::map<size_t, size_t> relations;

#define title_set class_database::first
#define student_set class_database::second
#define old_value adjustment::first
#define new_value adjustment::second

enum class USAGE
{
    argc,
    bad_file,
    open_error,
    bad_format,
};

[[noreturn]] void usage( const char* executable, USAGE _error )
{
    std::cerr << "Invalid call to \"" << executable << "\n";
    std::cerr << "The program expects 2 arguments:\n";
    std::cerr << "The executable, and a CSV file for it to open.\n\n";

    if (_error == USAGE::argc) std::cerr << "You did not send in the proper number of arguments.\n";
    if (_error == USAGE::bad_file) std::cerr << "The file you sent in does not appear to be a CSV.\n";
    if (_error == USAGE::open_error) std::cerr << "That file could not be opened, double check it exists and try again.\n";
    if (_error == USAGE::bad_format) std::cerr << "The file you sent in had an unexpected format!\n";
    exit(EXIT_FAILURE);
}

bool check_filename( std::string _filename )
{ 
    size_t extension = _filename.rfind('.');

    if (extension == _filename.npos) return false;
    if (_filename.substr(extension) != ".csv") return false;

    return true;
}

void validate( int argc, char** argv )
{
    if (argc != 2) throw USAGE::argc;
    if (!check_filename(argv[1])) throw USAGE::bad_file;
}

template < typename _Iterable, typename _Type >
_Iterable find( _Iterable _begin, _Iterable _end, _Type& _value )
{
    for (; _begin != _end; ++_begin )
        if (*_begin == _value) break;

    return _begin;
}

template < typename _Elem >
std::basic_string<_Elem> get_lowercase(std::basic_string<_Elem> _string)
{
    for ( _Elem& element : _string ) element = tolower(element);
    return _string;
}

string_set parse_line( std::string _data )
{
    std::stringstream data_stream(_data);
    string_set parsed;
    std::string data_point;
    while ( std::getline(data_stream >> std::ws, data_point, ',') ) parsed.push_back(data_point);
    return parsed;
}

// parse_username must throw an error if the username could not be parsed.
class_database parse( std::ifstream& _input, std::string (*parse_username)(std::string) )
{
    class_data students;
    std::string line;
    if (!std::getline(_input >> std::ws, line, '\n')) throw USAGE::bad_format;

    string_set titles = parse_line(line);

    while (std::getline(_input >> std::ws, line, '\n'))
    {
        student next_student;
        string_set student_data = parse_line(line);
        for ( std::string data_point : student_data )
        {
            try { next_student.grades.push_back( std::stoi(data_point) ); }
            catch(...)
            {
                try { next_student.username = parse_username(data_point); }
                catch (...) {}
            }
        }

        students.push_back(next_student);
    }

    return std::make_pair( titles, students );
}

adjustment get_replacement( std::string _replacement )
{
    size_t splitter = _replacement.find("->");
    if (splitter == std::string::npos) throw std::invalid_argument("String is missing ->");

    return std::make_pair( _replacement.substr(0, splitter), _replacement.substr(splitter+2) );
}

adjusments prompt_replacements()
{
    std::cout << "Please type text adjustments in the format: [old]->[new]\n";
    std::cout << "When you are done typing adjustments, type \"done\"\n";

    std::string typed;
    adjusments replacements;
    
    do
    {
        std::getline(std::cin, typed);

        try { replacements.push_back(get_replacement(typed)); }
        catch (...) { if (typed != "done") std::cerr << "Something wasn't quite right.\n"; }
    }
    while (typed != "done");
    
    return replacements;
}

void raw_replace( std::string& _replaceable, const std::string& _old, const std::string& _new )
{
    size_t replace_start = _replaceable.find(_old);

    if (replace_start != std::string::npos)
        _replaceable = _replaceable.substr(0, replace_start) + _new +   \
                       _replaceable.substr(replace_start + _old.size());
}

void raw_replace_set( string_set& _replace_set, adjustment _replacement )
{
    for ( std::string& replaceable : _replace_set )
        raw_replace(replaceable, _replacement.old_value, _replacement.new_value);
}

string_set parse_command( std::string& _command )
{
    size_t start_position = 0, end_position = 0;
    string_set broken_down;
    while(end_position < _command.length())
    {
        start_position = _command.find('$', end_position);
        if (start_position != end_position)
            broken_down.push_back(_command.substr(end_position, start_position-end_position));
        if (start_position == std::string::npos) break;
        
        if (_command[start_position+1] != '(')
            end_position = start_position + 2;
        else
        {
            end_position = _command.find(')');
            if (end_position == std::string::npos)
                throw std::invalid_argument("Command was not given an end [')']!");
        }

        broken_down.push_back(_command.substr(start_position, end_position-start_position));
    } 
    return broken_down;
}

void update_variable( string_set& _set, std::string _variable, std::string _replacement )
{
    string_set::iterator to_replace = find<string_set::iterator, std::string>(_set.begin(), _set.end(), _variable);

    if (to_replace != _set.end()) *to_replace = _replacement;
}

std::string build( const string_set& _pieces )
{
    std::string result;
    for ( const std::string& piece : _pieces ) result += piece;
    return result;
}

std::string generate( const std::string& _replaceable, const string_set& _old, const string_set& _new )
{
    string_set breakdown = _new;
    std::string working = _replaceable;

    for ( size_t index = 0; index < _old.size(); ++index )
    {
        const std::string& partial = _old[index];
        if ( partial[0] != '$' )
        {
            if (working.find(partial) != 0) return _replaceable;
            else working = working.substr(partial.length());
        }
        else
        {
            size_t end;
            if (index + 1 < _old.size())
                end = working.find(_old[index+1]);
            else 
                end = working.size();

            update_variable(breakdown, partial, working.substr(0, end));

            if (end != std::string::npos) working = working.substr(end);
            else working = "";
        }
    }

    return build(breakdown);
}

void functional_replace_set( string_set& _replace_set, adjustment _replacement )
{
    string_set lookup_format = parse_command(_replacement.old_value);
    string_set output_format = parse_command(_replacement.new_value);

    for ( std::string& replaceable : _replace_set )
    {
        replaceable = generate(replaceable, lookup_format, output_format);
    }
}

void replace_set( string_set& _replace_set, adjustment _replacement )
{
    if ( _replacement.old_value.find('$') == std::string::npos ) 
        raw_replace_set(_replace_set, _replacement);
    
    else functional_replace_set(_replace_set, _replacement);
}

void process_replacements( string_set& _replace_set, adjusments _replacements )
{
    for ( adjustment replacement : _replacements )
        replace_set(_replace_set, replacement);
}

std::string email_to_username( std::string _email )
{
    size_t at_symbol = _email.find('@');

    if( at_symbol == std::string::npos ) throw std::invalid_argument("Value is not an email!");

    return _email.substr(0, at_symbol);
}

void print_set( string_set& _set, std::ostream& os = std::cout )
{
    for ( std::string& value : _set ) os << value << ", ";
    os << "\n\n";
}

std::string get_change( std::string& _modifiable )
{
    std::cout << "How do you want to change this string? \"" << _modifiable << "\"\n";
    std::cout << "Type \"REMOVE\" to remove it, anything else to change it.\n";

    std::string choice;
    std::getline(std::cin, choice);

    return choice;
}

bool make_change( string_set& _header )
{
    string_set::iterator selected;
    std::string selection;
    std::cout << "The set currently looks like:\n";
    print_set(_header);
    std::cout << "Please select which string you would like to change from the list:\n";
    std::cout << "You may either type the string itself, or the corrisponding index.\n";

    std::getline(std::cin >> std::ws, selection);

    try
    {
        size_t index = std::stoull(selection) - 1;
        if (index < 0 || index >= _header.size()) return false;
        
        selected = _header.begin() + index;
    }
    catch (...)
    {
        selected = find<string_set::iterator, std::string>(_header.begin(), _header.end(), selection);
        if (selected == _header.end()) return false;
    }
    std::string change = get_change(*selected);

    if (change == "REMOVE") _header.erase(selected);
    else *selected = change;

    return true;
}

void reformat_header( string_set& _header )
{
    std::cout << "Would you like to make any changes to the header? (y/n)\n";
    char response;
    std::cin >> response;
    
    while (tolower(response) != 'n')
    {
        make_change(_header);

        std::cout << "Here are your changes:\n";
        print_set(_header);

        std::cout << "\nWould you like to continue making changes? (y/n)\n";
        std::cin >> response;
    }
}


void print_database( class_database _database, std::ostream& os = std::cout )
{
    string_set& titles = _database.title_set;
    class_data& students = _database.student_set;

    print_set(titles, os);
    
    for ( student student_data : students )
    {
        os << student_data << ",#\n";
    }
}

int main( int argc, char** argv )
{
    class_database database;

    try
    {
        validate(argc, argv);
        std::ifstream input(argv[1]);
        if (!input.is_open()) throw USAGE::open_error;
        database = parse(input, email_to_username);
    }
    catch ( USAGE& _usage_error ) { usage(argv[0], _usage_error); }

    std::cout << "File has been successfully opened and parsed!\n\n";

    print_database(database);
    std::cout << "\n\n";
    print_set(database.title_set);
    adjusments replacements = prompt_replacements();

    process_replacements(database.title_set, replacements);

    print_set(database.title_set);

    reformat_header(database.title_set);

    try
    {
        std::ofstream output(argv[1]);
        print_database(database, output);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}