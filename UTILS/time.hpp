#pragma once
#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class time
{
public:
    using time_t = unsigned long long;

private:
    enum month
    {
        January = 1,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        October,
        November,
        December,
    };

public:
    time() : _time() {}
    time( const time& ) = default;
    time( time&& ) = default;
    time( const time_t& __time ) : _time( __time ) {}
    time( time_t&& __time ) : _time( __time ) {}

private:
    void convert_date_time( const std::string& __str_date );
    void convert_clock_time( const std::string& __str_clock );

public:
    void set_time( const std::string& __str_time );
    void set_time( time_t __time );

private:
    const char* get_date_time();
    const char* get_clock_time();

public:
    const char* get_time();
    time_t raw_time();

private:
    time_t _time;
};

#endif // TIME_HPP