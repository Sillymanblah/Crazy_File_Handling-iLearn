#pragma once
#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include <array>

class time
{
private:
    using short_time        = unsigned short;
    using time_value        = unsigned long;
    using byte              = unsigned char;

    // Must be: years, months, days, hours, minutes.
    using time_breakdown    = std::array< short_time, 5 >;

public:
    using raw_time          = time_t;

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
    enum class precision
    {
        milliseconds,
        centiseconds,
        deciseconds,
        seconds,
        minutes,
        hours,
        days,
        months,
        years,
    };

public:
    time() : _time() {}
    time( const time& ) = default;
    time( time&& ) = default;
    time( const time_value& __time ) : _time( __time ) {}
    time( time_value&& __time ) : _time( __time ) {}

private:
    // These 3 functions are unused!
    // This is technically a compile time constant if we know year.
    constexpr static inline bool is_leap_year( short_time __year );
    constexpr static inline short_time get_days_in_year( short_time __year );
    constexpr static inline short_time get_days_in_month( short_time __month, short_time __year );

    constexpr static inline short_time find_month( short_time& __days );
    constexpr static inline void break_raw_time( const raw_time& __time, const precision& __precision, time_breakdown& __parts );
    constexpr static inline short_time build_time( const time_breakdown& __parts );

public:
    void set_time( const raw_time& __time, const precision& __precision = precision::seconds );

private:
    constexpr static inline short_time isolate( const time_value& __time, const time_value& __mask, const byte& __bitshift = no_shift );

    constexpr static inline short_time get_year( const time_value& __time );
    constexpr static inline short_time get_month( const time_value& __time );
    constexpr static inline short_time get_day( const time_value& __time );
    constexpr static inline short_time get_hour( const time_value& __time );
    constexpr static inline short_time get_minute( const time_value& __time );

public:
    short_time get_time( const precision& __precision );

private:
    constexpr static inline std::string get_am_pm( const short_time& __hour );
    constexpr static inline std::string get_month( const short_time& __month );
    static inline std::string get_short_month( const short_time& __month );
    static inline std::string construct_time_string( const time_breakdown& __parts );

    static inline short_time days_before_month( const short_time& month );
    static inline raw_time construct_raw_time( const time_breakdown& __parts, const precision& __precision );

    static inline time_breakdown break_time( const time_value& __time );

public:
    std::string get_time_string();
    raw_time get_raw_time( const precision& __precision = precision::seconds );

private:
    time_value _time;

    // This is probably the easiest way to handle these values:
    // NOTE: These could also be enumerated values. Might refactor later.

    constexpr static inline short_time      max_years               = 65535;        // Permitted max years
    constexpr static inline time_value      year_mask               = 0x0FF00000;   // 16 bits required
    constexpr static inline byte            year_shift              = 20;           // Num bits to the right
    constexpr static inline byte            breakdown_year          = 0;            // Index of time_breakdown

    constexpr static inline short_time      max_months              = 12;           // Months in a year
    constexpr static inline time_value      month_mask              = 0x000F0000;   // 4 bits required
    constexpr static inline byte            month_shift             = 16;           // Num bits to the right
    constexpr static inline byte            breakdown_month         = 1;            // Index of time_breakdown

    constexpr static inline short_time      max_days[ max_months ]  = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // Max days per month for non-leap-years
    constexpr static inline time_value      day_mask                = 0x0000F800;   // 5 bits required
    constexpr static inline byte            day_shift               = 11;           // Num bits to the right
    constexpr static inline byte            breakdown_day           = 2;            // Index of time_breakdown

    constexpr static inline short_time      max_hours               = 24;           // Hours in a day
    constexpr static inline time_value      hour_mask               = 0x000007C0;   // 5 bits required
    constexpr static inline byte            hour_shift              = 6;            // Num bits to the right
    constexpr static inline byte            breakdown_hour          = 3;            // Index of time_breakdown

    constexpr static inline short_time      max_minutes             = 60;           // Minutes in an hour
    constexpr static inline time_value      minute_mask             = 0x0000003F;   // 6 bits required
    constexpr static inline byte            no_shift                = 0;            // No bit shift
    constexpr static inline byte            breakdown_minute        = 4;            // Index of time_breakdown

    constexpr static inline short_time      days_in_year            = 365;          // Total for non-leap-years
    constexpr static inline float           average_days_in_year    = 365.2425;     // Total for non-leap-years

    constexpr static inline short_time      seconds_per_minute      = 60;           // Used to convert time to seconds
    constexpr static inline short_time      deci                    = 10;           // For deeper time precision
    constexpr static inline short_time      centi                   = 100;          // For even deeper time precision
    constexpr static inline short_time      milli                   = 1000;         // For deepest time precision
};

#endif // TIME_HPP