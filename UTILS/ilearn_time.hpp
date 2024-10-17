#ifndef ILEARN_TIME_HPP
#define ILEARN_TIME_HPP

#include <sstream>
#include <string>

#define MONTH_NUM(_month_) if (month == #_month_) return _month_
#define NUM_MONTH(_month_) case _month_: return #_month_

namespace ilearn_time
{

    enum MONTHS : uint8_t
    {
        Jan = 1,
        Feb,
        Mar,
        Apr,
        May,
        Jun,
        Jul,
        Aug,
        Sep,
        Oct,
        Nov,
        Dec,
    };

    uint8_t get_month_number( const std::string& month )
    {
        MONTH_NUM(Jan);
        else MONTH_NUM(Feb);
        else MONTH_NUM(Mar);
        else MONTH_NUM(Apr);
        else MONTH_NUM(May);
        else MONTH_NUM(Jun);
        else MONTH_NUM(Jul);
        else MONTH_NUM(Aug);
        else MONTH_NUM(Sep);
        else MONTH_NUM(Oct);
        else MONTH_NUM(Nov);
        else MONTH_NUM(Dec);

        throw std::invalid_argument("Month is not a valid month of the year.");
    }
    std::string get_month( MONTHS month )
    {
        switch (month)
        {
            NUM_MONTH(Jan);
            NUM_MONTH(Feb);
            NUM_MONTH(Mar);
            NUM_MONTH(Apr);
            NUM_MONTH(May);
            NUM_MONTH(Jun);
            NUM_MONTH(Jul);
            NUM_MONTH(Aug);
            NUM_MONTH(Sep);
            NUM_MONTH(Oct);
            NUM_MONTH(Nov);
            NUM_MONTH(Dec);
        }

        throw std::invalid_argument("Number does not properly represent a month of the year."); // Should never get here, but just in case.
    }

    uint16_t get_days_before_month( uint8_t month, bool leap_year )
    {
        uint16_t days = 0;
        switch (month)
        {   // This intentionally spills the cases down.
            case 12: days += 30;
            case 11: days += 31;
            case 10: days += 30;
            case 9: days += 31;
            case 8: days += 31;
            case 7: days += 30;
            case 6: days += 31;
            case 5: days += 30;
            case 4: days += 31;
            case 3: days += 28;
                if (leap_year) days += 1;
            case 2: days += 31;
        }

        return days;
    }

    bool is_leap_year( uint16_t year )
    {
        return (year % 4) && (year % 100) || (year % 400);
    }

    uint32_t get_minutes_from_time( uint16_t time ) // Time is missing semi-colon (military time).
    {
        uint16_t hours = time / 100, minutes = time % 100;
        if (hours == 12) hours = 0;
        if (hours == 24) hours = 12;
        return (hours * 60) + minutes;
    }

    uint32_t get_days_from_year( const uint16_t& year )
    {
        uint32_t leap_days = year / 4 - year / 100 + year / 400;
        return (uint32_t)year * 365 + leap_days;
    }

    uint32_t get_minutes( const std::string& date_time )
    {
        std::string month, comma, am_pm;
        uint16_t year, days, minute_hours;
        uint32_t days_total, hours_total;

        std::stringstream(date_time) >> month >> days >> comma >> year >> minute_hours >> am_pm;
        days_total = days + get_days_before_month(get_month_number(month), is_leap_year(year));

        days_total += get_days_from_year(year);

        if (am_pm == "PM") minute_hours += 1200;

        return (days_total * 24 * 60) + get_minutes_from_time(minute_hours);
    }

    #define remove_days_if_over(_value_)    \
    if (days > _value_) {                   \
        days -= _value_;                    \
        ++months;                           \
    }                                       \
    else return months;

    uint8_t get_month_from_days( uint32_t& days, bool leap_year )
    {
        uint8_t months = 1;
        remove_days_if_over(31)
        if(leap_year) remove_days_if_over(29)
        else remove_days_if_over(28)
        remove_days_if_over(31)
        remove_days_if_over(30)
        remove_days_if_over(31)
        remove_days_if_over(30)
        remove_days_if_over(31)
        remove_days_if_over(31)
        remove_days_if_over(30)
        remove_days_if_over(31)
        remove_days_if_over(30)

        return months;
    }

// Accouting for leap years, and skipped leap years.
#define DAYS_IN_1_YEAR          (365)
#define DAYS_IN_4_YEARS         (DAYS_IN_1_YEAR * 4 + 1)
#define DAYS_IN_1_CENTURY       (DAYS_IN_4_YEARS * 25 - 1)
#define DAYS_IN_4_CENTURIES     (DAYS_IN_1_CENTURY * 4 + 1)
// Now for the function:
    uint16_t get_years_from_days( uint32_t& days )
    {
        uint16_t four_centuries, centuries, four_years, years;
        four_centuries = days / DAYS_IN_4_CENTURIES;                    days %= DAYS_IN_4_CENTURIES;
        centuries = (four_centuries * 4) + (days / DAYS_IN_1_CENTURY);  days %= DAYS_IN_1_CENTURY;
        four_years = (centuries * 25) + (days / DAYS_IN_4_YEARS);       days %= DAYS_IN_4_YEARS;
        years = (four_years * 4) + (days / DAYS_IN_1_YEAR);             days %= DAYS_IN_1_YEAR;
        return years;
    }

    std::string get_time_string( uint8_t hours, uint8_t minutes )
    {
        bool is_pm = hours > 12;
        hours = hours % 12;
        if (hours == 0) hours += 12;
        std::string hour_str(std::to_string(hours)), minute_str(std::to_string(minutes));
        if (hour_str.length() < 2) hour_str = "0" + hour_str;
        if (minute_str.length() < 2) minute_str = "0" + minute_str;
        return hour_str + ":" + minute_str + (is_pm ? " PM" : " AM");
    }
    std::string get_date_string( uint16_t years, uint8_t months, uint8_t days )
    {
        std::string date = get_month(MONTHS(months));
        date += ' ' + std::to_string(days);
        date += ", " + std::to_string(years);
        return date;
    }

    const std::string get_date_time( uint32_t time_minutes )
    {
        uint32_t time_hours = time_minutes / 60, days = time_hours / 24;
        uint8_t minutes = time_minutes % 60, hours = time_hours % 24, months;
        uint16_t years = get_years_from_days(days);
        months = get_month_from_days(days, is_leap_year(years));
        return get_date_string(years, months, days) + ' ' + get_time_string(hours, minutes);
    }
} // namespace ilearn_time

#endif // ILEARN_TIME_HPP