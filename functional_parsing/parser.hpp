#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>

template < class _Elem, class _Storage, class... _Types >
class basic_parser
{
private:
    static constexpr inline size_t num_types = sizeof( _Types... );

private:
    using _MyStream = std::basic_ifstream< _Elem >;
    using _MyString = std::basic_string< _Elem >;

    template < class _Fn >
    class function_set
    {
    public:
        inline const size_t& size() { return count; }
        inline constexpr size_t max_size() { return functions.size(); }

        inline bool is_full() { return count == functions.size(); }
        inline bool is_empty() { return count == 0; }

        const _Fn& at( size_t __index ) { return functions.at( __index ); }
        void append_function( _Fn __function ) { if ( !is_full() ) functions[ count++ ] = __function; }
        void pop_function() { if ( !is_empty() ) functions[ --count ] = static_cast< _Fn >( NULL ); }

    private:
        std::array< _Fn, num_types > functions;
        size_t count = 0;
    };

public:
    using storage = _Storage;

    using read_function = _MyStream& (*)( _MyStream&, _MyString& );
    using skip_function = _MyStream& (*)( _MyStream& );

    template < class _Ty > // Enforce that the type here is contained in (_Types...).
    using conversion_function = std::enable_if_t< std::__is_one_of< _Ty, _Types... >::value, _Ty > (*)( const _MyString& );
    using conversion_functions = std::tuple< conversion_function< _Types >... >;
    using store_function = void (*)( _Storage&, const _Types&... );

public:
    parser() = default;
    parser( const parser& ) = default;
    parser( parser&& ) = default;
    parser( read_function __reader, skip_function __skipper, store_function __pusher, conversion_functions __converters );
    parser( read_function __reader, skip_function __skipper, store_function __pusher, conversion_function< _Types >... __converters );

    void add_reader( read_function __reader );
    void add_skipper( skip_function __skipper );
    template < class _Ty >
    void add_converter( conversion_function< _Ty > __converter, size_t __index = num_types );
    void add_converters( conversion_function< _Types >... __converters );
    void add_converters( conversion_functions __converters );
    void add_pusher( store_function __pusher );

    _Storage parse( _MyStream& __filestream );

    _Storage parse( const std::string& __filename );
    _Storage parse( std::string&& __filename );
    _Storage parse( const char* __filename );

private:
    function_set< read_function > readers;
    function_set< skip_function > skippers;
    conversion_functions converters;
    store_function push_value;
};

template < class _Storage, class... _Types >
using parser = basic_parser< char, _Storage, _Types... >;
template < class _Storage, class... _Types >
using wparser = basic_parser< wchar_t, _Storage, _Types... >;

#endif // PARSER_HPP