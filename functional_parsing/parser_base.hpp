#pragma once
#ifndef PARSER_BASE_HPP
#define PARSER_BASE_HPP

#include <fstream>

template < class _Data, class _Elem, class _Traits = std::char_traits< _Elem > >
class parser_base
{
protected:
    using ifstream  = std::basic_ifstream< _Elem, _Traits >&;
    template < class _Alloc = std::allocator< _Elem > >;
    using string    = std::basic_string< _Elem, _Traits, _Alloc >&;
    using data_type = _Data;

public:
    virtual data_type parse( const char* __filename );
    template < class _Alloc = std::allocator< _Elem > >;
    virtual data_type parse( const string< _Alloc >& __filename );
    template < class _Alloc = std::allocator< _Elem > >;
    virtual data_type parse( string< _Alloc >&& __filename );
    virtual data_type parse( ifstream& __input );
};

#endif // PARSER_BASE_HPP