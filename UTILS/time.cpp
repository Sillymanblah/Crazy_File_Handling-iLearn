#include "time.hpp"
#include <stdexcept>

// Leap years every 4 years, skipped every 100, but not on the 400th
bool time::is_leap_year( short_time __year )
{ return !(__year % 4) && ( (__year % 100) || !(__year % 400) ); }

// Year has 365 days, +1 for leap years.
time::short_time time::get_days_in_year( short_time __year )
{ return is_leap_year( __year ) ? days_in_year + 1 : days_in_year; }


time::short_time time::get_days_in_month( short_time __month, short_time __year )
{ return ( __month == month::February ) ? ( is_leap_year( __year ) ? max_days[ __month ] + 1 : max_days[ __month ] ) : max_days[ __month ]; }

void time::set_time( const std::string& __str_time )
{
    // still need to write!
}

void time::set_time( time_value __time )
{
    // still need to write!
}

time::short_time time::isolate( const time_value& __isolator, const unsigned char& bit_shift )
{ return (_time & __isolator) >> bit_shift; }

time::short_time time::get_year()
{ return isolate( year_mask, year_shift ); }
time::short_time time::get_month()
{ return isolate( month_mask, month_shift ); }
time::short_time time::get_day()
{ return isolate( day_mask, day_shift ); }
time::short_time time::get_hour()
{ return isolate( hour_mask, hour_shift ); }
time::short_time time::get_minute()
{ return isolate( minute_mask ); }

std::string time::get_am_pm( short_time __hour )
{
    if ( __hour < 12 ) return "AM";
    else return "PM";
}

std::string time::get_month( short_time __month )
{
    switch ( __month )
    {
        case month::January:    return "January";
        case month::February:   return "February";
        case month::March:      return "March";
        case month::April:      return "April";
        case month::May:        return "May";
        case month::June:       return "June";
        case month::July:       return "July";
        case month::August:     return "August";
        case month::September:  return "September";
        case month::October:    return "October";
        case month::November:   return "November";
        case month::December:   return "December";
        default: throw std::invalid_argument( "Time was not a valid month!" );
    }
}

std::string time::get_short_month( short_time __month )
{ return get_month( __month ).substr( 0, 3 ); }

std::string time::construct_time_string( const std::array< short_time, 5 >&& __parts )
{
    using std::to_string;
    const auto& [ year, month, day, hour, minute ] = __parts;

    return get_short_month( month ) + ' ' + to_string( day )
        + ", " + to_string( year ) + ' ' + to_string( hour % 12 )
        + to_string( minute ) + ' ' + get_am_pm( hour );
}

std::array< time::short_time, 5 > time::break_time()
{ return { get_year(), get_month(), get_day(), get_hour(), get_minute() };}

std::string time::get_time()
{ return construct_time_string( break_time() ); }

time::time_value time::raw_time()
{ return _time; }