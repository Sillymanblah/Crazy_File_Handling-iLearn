#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>

template < class _Elem, class _Storage, class... _Types >
class basic_parser
{
private:
    using _MyStream = std::basic_ifstream< _Elem >;
    using _MyString = std::basic_string< _Elem >;

public:
    using read_function = _MyStream& (*)( _MyStream&, _MyString& );
    using skip_function = _MyStream& (*)( _MyStream& );

    template < class _Ty > // Enforce that the type here is contained in (_Types...).
    using conversion_function = std::enable_if< std::__is_one_of< _Ty, _Types... >::value, _Ty > (*)( const _MyString& );
    using conversion_functions = std::tuple< conversion_function< _Types >... >;
    using store_function = void (*)( _Storage, _Types... );

public:
    parser() = default;
    parser( const parser& ) = default;
    parser( parser&& ) = default;
    parser( read_function __reader, skip_function __skipper, store_function __pusher, conversion_functions __converters );
    parser( read_function __reader, skip_function __skipper, store_function __pusher, conversion_function< _Types >... __converters );

    void add_reader( read_function __reader );
    void add_skipper( skip_function __skipper );
    template < class _Ty >
    void add_converter( conversion_function< _Ty > __converter, size_t __index = sizeof( _Types... ) );
    void add_converters( conversion_function< _Types >... __converters );
    void add_converters( conversion_functions __converters );
    void add_pusher( store_function __pusher );

    _Storage parse( _MyStream& __filestream );

    _Storage parse( const std::string& __filename );
    _Storage parse( std::string&& __filename );
    _Storage parse( const char* __filename );

private:
    read_function* readers;
    skip_function skipper;
    conversion_functions converters;
    store_function push_value;
};

#endif // PARSER_HPP