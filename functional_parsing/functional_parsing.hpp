#pragma once
#ifndef FUNCTIONAL_PARSING_HPP
#define FUNCTIONAL_PARSING_HPP
#endif // FUNCTIONAL_PARSING_HPP

#include <functional>
#include <string>
#include <vector>

template <class _Elem>
using basic_string_conditional  =   std::function<bool(const std::basic_string<_Elem>&)>;
template <class _Elem>
using basic_reader              =   std::function<bool(std::basic_istream<_Elem>&, std::basic_string<_Elem>&)>;
template <class _Elem>
using basic_parsed_string       =   std::vector<std::basic_string<_Elem>>;
template <class _Elem>
using basic_parser              =   std::function<basic_parsed_string<_Elem>(std::basic_istream<_Elem>&)>;

typedef basic_parser<char>      sparser;
typedef basic_parser<wchar_t>   wsparser;
#ifdef __cpp_lib_char8_t
typedef basic_parser<char8_t>   u8sparser;
#endif // __cpp_lib_char8_t
typedef basic_parser<char16_t>  u16sparser;
typedef basic_parser<char32_t>  u32sparser;

typedef basic_reader<char>      sreader;
typedef basic_reader<wchar_t>   wsreader;
#ifdef __cpp_lib_char8_t
typedef basic_reader<char8_t>   u8sreader;
#endif // __cpp_lib_char8_t
typedef basic_reader<char16_t>  u16sreader;
typedef basic_reader<char32_t>  u32sreader;

typedef basic_parsed_string<char>      parsed_string;
typedef basic_parsed_string<wchar_t>   parsed_wstring;
#ifdef __cpp_lib_char8_t
typedef basic_parsed_string<char8_t>   parsed_u8string;
#endif // __cpp_lib_char8_t
typedef basic_parsed_string<char16_t>  parsed_u16string;
typedef basic_parsed_string<char32_t>  parsed_u32string;

typedef basic_string_conditional<char>      string_conditional;
typedef basic_string_conditional<wchar_t>   wstring_conditional;
#if __cpp_lib_char8_t
typedef basic_string_conditional<char8_t>   u8string_conditional;
#endif
typedef basic_string_conditional<char16_t>  u16string_conditional;
typedef basic_string_conditional<char32_t>  u32string_conditional;


template <class _Elem>
basic_parsed_string<_Elem> manual_parse
(
    std::basic_istream<_Elem>& input,
    basic_string_conditional<_Elem> start,
    basic_string_conditional<_Elem> stop,
    basic_reader<_Elem> reader
)
{
    size_t current_position = 0; // If the functions are the same, we have to start parsing the next immediately.
    std::basic_string<_Elem> output, bucket = "";
    basic_parsed_string<_Elem> parsed;

    bool is_reading = false;

    while (reader(input, output))
    {
        if (is_reading)
        {
            is_reading = !stop(output);

            if (!is_reading)
            {
                parsed.push_back(bucket);
                bucket = "";
            }
        }
        if (!is_reading)
        {
            is_reading = start(output);
        }
        if (is_reading) bucket += output;
    }
    if (bucket.size()) parsed.push_back(bucket);

    return parsed;
}

template <class _Elem>
basic_parser<_Elem> build_parser
(
    basic_string_conditional<_Elem> start,
    basic_string_conditional<_Elem> stop,
    basic_reader<_Elem> reader
)
{
    return [start, stop, reader](std::basic_istream<_Elem>& input)
    { return manual_parse(input, start, stop, reader); };
}

template <class _Elem, class _Pr = std::equal_to<std::basic_string<_Elem>>>
basic_string_conditional<_Elem> build_string_conditional(const std::basic_string<_Elem>& lookup)
{
    return [lookup](const std::basic_string<_Elem>& value)
    {
        _Pr comparer;
        return comparer(value, lookup);
    };
}