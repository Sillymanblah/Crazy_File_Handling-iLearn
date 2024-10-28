#ifndef HTML_PARSING_HPP
#define HTML_PARSING_HPP

// NOTE TO WHOMEVER READS THIS!
/*
 - This file was rewritten for the current specific usage but will be completely
 - overhauled again when I figure out how to improve the functional parsing file.
 - For the time being this works for what I need it to do.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <list>
#include <stack>
#include <map>

using parsed_strings = std::vector< std::string >;

void read_till_next_command( std::ifstream& file, std::string& out_string )
{ std::getline( file >> std::ws, out_string, '<' ); }

void skip_to_next_command( std::ifstream& file )
{ file.ignore( std::numeric_limits< std::streamsize >::max(), '<' ); }

enum class command_type : unsigned char
{
    html = 1,
    body,
    table,
    table_row,
    table_data,
    bold,

    end_html = 11,
    end_body,
    end_table,
    end_table_row,
    end_table_data,
    end_bold,

    paragraph = 51,
    font,
    line_break,

    unk = 255,
};

command_type get_end( const command_type& type )
{
    switch ( type )
    {
        case command_type::html: return command_type::end_html;
        case command_type::body: return command_type::end_body;
        case command_type::table: return command_type::end_table;
        case command_type::table_data: return command_type::end_table_data;
        case command_type::table_row: return command_type::end_table_row;
        case command_type::bold: return command_type::end_bold;

        // Not good, but for now works.
        default: throw command_type::unk;
    }
}

std::string command_to_string( const command_type& type )
{
    switch ( type )
    {
        case command_type::html: return "html";
        case command_type::body: return "body";
        case command_type::table: return "table";
        case command_type::table_data: return "tr";
        case command_type::table_row: return "td";
        case command_type::bold: return "b";

        case command_type::end_html: return "/html";
        case command_type::end_body: return "/body";
        case command_type::end_table: return "/table";
        case command_type::end_table_data: return "/tr";
        case command_type::end_table_row: return "/td";
        case command_type::end_bold: return "/b";

        case command_type::paragraph: return "p";
        case command_type::font: return "font";
        case command_type::line_break: return "br";

        case command_type::unk: return "";

        // Not good, but for now works.
        default: throw command_type::unk;
    }
}

command_type string_to_command( const std::string& type_str )
{
    if ( type_str == "html" ) return command_type::html;
    else if ( type_str == "body" ) return command_type::body;
    else if ( type_str == "table" ) return command_type::table;
    else if ( type_str == "tr" ) return command_type::table_data;
    else if ( type_str == "td" ) return command_type::table_row;
    else if ( type_str == "b" ) return command_type::bold;

    else if ( type_str == "/html" ) return command_type::end_html;
    else if ( type_str == "/body" ) return command_type::end_body;
    else if ( type_str == "/table" ) return command_type::end_table;
    else if ( type_str == "/tr" ) return command_type::end_table_data;
    else if ( type_str == "/td" ) return command_type::end_table_row;
    else if ( type_str == "/b" ) return command_type::end_bold;

    else if ( type_str == "p" ) return command_type::paragraph;
    else if ( type_str == "font" ) return command_type::font;
    else if ( type_str == "br" ) return command_type::line_break;

    else return command_type::unk;
}

parsed_strings break_command( const std::string& command_str )
{
    parsed_strings command_parts;
    std::string part;

    std::stringstream command_stream( command_str );

    while ( command_stream >> part )
        command_parts.push_back( part );

    return command_parts;
}

bool command_requires_end( const command_type& type )
{
    return !(
        type == command_type::line_break ||
        type == command_type::font ||
        type == command_type::paragraph ||
        type == command_type::unk
    );
}

// Do not like this recursion, maybe use some kind of stack to track commands.
void get_table( std::ifstream& file, parsed_strings& table )
{
    std::stack< command_type > active_commands;
    std::string command, table_data;

    while ( std::getline( file, command, '>' ) )
    {
        parsed_strings command_parts = break_command( command );
        command_type type = string_to_command( command_parts[0] );
        
        if ( active_commands.size() > 0 && type == get_end( active_commands.top() ) ) // If we have found the end of a command, jump out to previous recursive call at the start of next command
        {
            active_commands.pop();
            skip_to_next_command( file );
            continue;
        }
        
        read_till_next_command( file, table_data ); // Read to the start of the next command for data.

        if ( table_data.size() > 1 )
            table.push_back( table_data );

        if ( command_requires_end( type ) )
            active_commands.push( type );
    }
}

parsed_strings html_parser( std::ifstream& file )
{
    parsed_strings data;

    skip_to_next_command( file );
    get_table( file, data );

    return data;
}

#endif // HTML_PARSING_HPP