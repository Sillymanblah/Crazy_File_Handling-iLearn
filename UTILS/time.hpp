#pragma once
#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include <array>

class time
{
private:
    using short_time        = unsigned int;
    using time_value        = unsigned long;

public:
    // Must be: years, months, days, hours, minutes.
    using time_breakdown    = std::array< short_time, 5 >;

private:
    enum month
    {
        January,
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
    time( const time_value& __time ) : _time( __time ) {}
    time( time_value&& __time ) : _time( __time ) {}

private:
    // This is technically a compile time constant if we know year.
    constexpr static inline bool is_leap_year( short_time __year );
    constexpr static inline short_time get_days_in_year( short_time __year );
    constexpr static inline short_time get_days_in_month( short_time __month, short_time __year );

public:
    void set_time( const std::string& __str_time ); // Definitely need helpers for this function!
    void set_time( time_value __time );

private:
    short_time isolate( const time_value& __isolator, const unsigned char& bitshift = 0 );

public:
    short_time get_year();
    short_time get_month();
    short_time get_day();
    short_time get_hour();
    short_time get_minute();


private:
    static inline std::string get_am_pm( short_time __hour );
    static inline std::string get_month( short_time __month );
    static inline std::string get_short_month( short_time __month );

    static inline std::string construct_time_string( const time_breakdown&& __parts );
    time_breakdown break_time();

public:
    std::string get_time();
    time_value raw_time();

private:
    time_value _time;

    // This is probably the easiest way to handle these values:

    constexpr static inline short_time      max_years               = 65535;        // Permitted max years
    constexpr static inline time_value      year_mask               = 0x0FF00000;   // 16 bits required
    constexpr static inline unsigned char   year_shift              = 20;           // Num bits to the right
    constexpr static inline short_time      max_months              = 12;           // Months in a year
    constexpr static inline time_value      month_mask              = 0x000F0000;   // 4 bits required
    constexpr static inline unsigned char   month_shift             = 16;           // Num bits to the right
    constexpr static inline short_time      max_days[ max_months ]  = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // Max days per month for non-leap-years
    constexpr static inline time_value      day_mask                = 0x0000F800;   // 5 bits required
    constexpr static inline unsigned char   day_shift               = 11;           // Num bits to the right
    constexpr static inline short_time      max_hours               = 24;           // Hours in a day
    constexpr static inline time_value      hour_mask               = 0x000007C0;   // 5 bits required
    constexpr static inline unsigned char   hour_shift              = 6;            // Num bits to the right
    constexpr static inline short_time      max_minutes             = 60;           // Minutes in an hour
    constexpr static inline time_value      minute_mask             = 0x0000003F;   // 6 bits required

    constexpr static inline short_time      days_in_year            = 365;          // Total for non-leap-years
};

#endif // TIME_HPP