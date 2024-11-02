#include "time.hpp"
#include <stdexcept>

// Leap years every 4 years, skipped every 100, but not on the 400th
bool time::is_leap_year( short_time __year )
{ return !(__year % 4) && ( (__year % 100) || !(__year % 400) ); }

// Year has 365 days, +1 for leap years.
time::short_time time::get_days_in_year( short_time __year )
{ return is_leap_year( __year ) ? days_in_year + 1 : days_in_year; }

// February has one more day on leap years.
time::short_time time::get_days_in_month( short_time __month, short_time __year )
{ return ( __month == month::February ) ? ( is_leap_year( __year ) ? max_days[ __month ] + 1 : max_days[ __month ] ) : max_days[ __month ]; }

// Finds the month based on number of days into the year, and leaves __days as the number of days remaining (so days in the month)
time::short_time time::find_month( short_time& __days )
{
    for ( short_time month = 0; month < max_months; ++month )
    {
        if ( __days > max_days[ month ] ) __days -= max_days[ month ];
        else return month;
    }
}

void time::break_raw_time( const raw_time& __time, const precision& __precision, time_breakdown& __parts )
{
    switch ( __precision )
    {
    case precision::years:
        if ( __time > max_years ) throw std::invalid_argument( "The raw time entered has a time larger than the maximum allowed time!" );
        __parts[ breakdown_year ] = static_cast< short_time >( __time );
        break;

    case precision::months:
        break_raw_time( __time / max_months, precision::years, __parts );
        __parts[ breakdown_month ] = static_cast< short_time >( __time % max_months );
        break;

    case precision::days:
        break_raw_time( __time / average_days_in_year, precision::years, __parts );
        __parts[ breakdown_month ] = find_month( __parts[ breakdown_day ] = __time - __parts[ breakdown_year ] * average_days_in_year );
        break;

    case precision::hours:
        break_raw_time( __time / max_hours, precision::days, __parts );
        __parts[ breakdown_hour ] = static_cast< short_time >( __time % max_hours );
        break;

    case precision::minutes:
        break_raw_time( __time / max_minutes, precision::hours, __parts );
        __parts[ breakdown_hour ] = static_cast< short_time >( __time % max_minutes );
        break;

    case precision::seconds:
        break_raw_time( __time / seconds_per_minute, precision::minutes, __parts );
        break;

    case precision::deciseconds:
        break_raw_time( __time / deci, precision::seconds, __parts );
        break;

    case precision::centiseconds:
        break_raw_time( __time / centi, precision::seconds, __parts );
        break;

    case precision::milliseconds:
        break_raw_time( __time / milli, precision::seconds, __parts );
        break;

    default:
        throw std::invalid_argument( "The value of precision seems to be invalid!" );
    }
}

time::short_time time::build_time( const time_breakdown& __parts )
{ return __parts[ breakdown_year ] << year_shift | __parts[ breakdown_month ] << month_shift | __parts[ breakdown_day ] << day_shift | __parts[ breakdown_hour ] << hour_shift | __parts[ breakdown_minute ]; }

void time::set_time( const raw_time& __time, const precision& __precision )
{
    time_breakdown parts;
    break_raw_time( __time, __precision, parts );
    _time = build_time( parts );
}

time::short_time time::isolate( const time_value& __time, const time_value& __mask, const byte& __bitshift )
{ return (__time & __mask) >> __bitshift; }

time::short_time time::get_year( const time_value& __time )
{ return isolate( __time, year_mask, year_shift ); }
time::short_time time::get_month( const time_value& __time )
{ return isolate( __time, month_mask, month_shift ); }
time::short_time time::get_day( const time_value& __time )
{ return isolate( __time, day_mask, day_shift ); }
time::short_time time::get_hour( const time_value& __time )
{ return isolate( __time, hour_mask, hour_shift ); }
time::short_time time::get_minute( const time_value& __time )
{ return isolate( __time, minute_mask ); }

time::short_time time::get_time( const precision& __precision )
{
    switch ( __precision )
    {
    case precision::years: return get_year( _time );
    case precision::months: return get_month( _time );
    case precision::days: return get_day( _time );
    case precision::hours: return get_hour( _time );
    case precision::minutes: return get_minute( _time );

    // We do not store values for seconds or below.
    case precision::seconds:
    case precision::milliseconds:
    case precision::centiseconds:
    case precision::deciseconds:
        return 0;

    default:
        throw std::invalid_argument( "The value of precision seems to be invalid!" );
    }
}

std::string time::get_am_pm( const short_time& __hour )
{
    if ( __hour < 12 ) return "AM";
    else return "PM";
}

std::string time::get_month( const short_time& __month )
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

std::string time::get_short_month( const short_time& __month )
{ return get_month( __month ).substr( 0, 3 ); }

time::short_time time::military_to_standard( const short_time& __hour )
{ return (__hour + 11) % 12 + 1; }

std::string time::construct_time_string( const time_breakdown& __parts )
{
    const auto& [ year, month, day, hour, minute ] = __parts;

    short_time val = -1;

    using std::to_string;
    return get_short_month( month ) + ' ' + to_string( day + 1 )
        + ", " + to_string( year ) + ' ' + to_string( military_to_standard( hour ) )
        + ':' + to_string( minute ) + ' ' + get_am_pm( hour );
}

time::short_time time::days_before_month( const short_time& __month )
{
    short_time total_days = 0;
    for ( short_time month = 0; month < __month; ++month )
        total_days += max_days[ month ];
    return total_days;
}

time::raw_time time::construct_raw_time( const time_breakdown& __parts, const precision& __precision )
{
    raw_time time_val = 0;
    const auto& [ year, month, day, hour, minute ] = __parts;

    switch ( __precision )
    {
        case precision::years:
            return year;
        case precision::months:
            return year * max_months + month;
        case precision::days:
            return year * average_days_in_year + days_before_month( month ) + day;
        case precision::hours:
            return ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour; // If we switch to recursion this will look cleaner.
        case precision::minutes:
            return ( ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour ) * max_minutes + minute;
        case precision::seconds:
            return ( ( ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour ) * max_minutes + minute ) * seconds_per_minute;
        case precision::deciseconds:
            return ( ( ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour ) * max_minutes + minute ) * seconds_per_minute * deci;
        case precision::centiseconds:
            return ( ( ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour ) * max_minutes + minute ) * seconds_per_minute * centi;
        case precision::milliseconds:
            return ( ( ( year * average_days_in_year + days_before_month( month ) + day ) * max_hours + hour ) * max_minutes + minute ) * seconds_per_minute * milli;
    
        default:
            throw std::invalid_argument( "The value of precision seems to be invalid!" );
    }
}

time::time_breakdown time::break_time( const time_value& __time )
{ return { get_year( __time ), get_month( __time ), get_day( __time ), get_hour( __time ), get_minute( __time ) };}

std::string time::get_time_string()
{ return construct_time_string( break_time( _time ) ); }

time::raw_time time::get_raw_time( const precision& __precision )
{ return construct_raw_time( break_time( _time ), __precision ); }