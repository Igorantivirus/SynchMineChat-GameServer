#pragma once

#include <chrono>
#include <string>
#include <map>

struct DateTime
{
    std::chrono::year_month_day date{};
    std::chrono::hh_mm_ss<std::chrono::milliseconds> time{};

public:

    bool operator<(const DateTime& other) const
    {
        if(date == other.date)
            return time.to_duration() < other.time.to_duration();
        return date < other.date;
    }
    
    DateTime() = default;

    static DateTime fromLogString(const std::string& str)
    {
        DateTime res;
        if(str.size() < 24)
            return;
        


    }

private:

    static unsigned short parseMonth(const std::string s)
    {
        // std::chrono::January

        // std::map<std::string, unsigned short> converter = 
        // {
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        //     {},
        // };
    }

};