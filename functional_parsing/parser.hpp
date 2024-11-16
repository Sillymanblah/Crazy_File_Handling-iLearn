
#include <fstream>

template < class _Elem, class _Storage, class... _Types >
class parser
{
public:
    using _MyStream = std::basic_fstream< _Elem >;
    using _MyString = std::basic_string< _Elem >;

    using read_function = _MyStream& (*)( _MyStream&, _MyString& );
    using skip_function = _MyStream& (*)( _MyStream& );

    template < class _Ty > // Enforce that the type here is contained in (_Types...).
    using conversion_function = _Ty (*)( const _MyString& );
    using store_function = void (*)( _Storage, _Types... );
    using emplace_function = void (*)( _Storage, _Types... );
    using new_emplace_function = typename _Storage::value_type& (*)( _Storage, _Types... );

public:
    parser() = default;
    parser( const parser& );
    parser( parser&& );

    void add_reader( read_function fn );
    void add_skipper( skip_function fn );
    template < class _Ty >
    void add_converter( conversion_function< _Ty > fn, size_t index = sizeof( _Types... ) );
    void add_converters( std::tuple< conversion_function< _Types >... > fns );
    void add_pusher( store_function fn );
    void add_emplacer( emplace_function fn );
    void add_emplacer( new_emplace_function fn );

    _Storage parse( _MyStream& __filestream );

    _Storage parse( const std::string& __filename );
    _Storage parse( std::string&& __filename );
    _Storage parse( const char* __filename );

private:
    read_function* readers;
    skip_function skipper;
    std::tuple< conversion_function< _Types >... > converters;
    store_function push_value;
    union emplace
    {
        new_emplace_function new_fn;
        emplace_function old_fn;
    } emplace_value;

};

