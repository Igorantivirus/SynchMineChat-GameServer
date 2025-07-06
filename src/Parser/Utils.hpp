#pragma once

#include <fstream>
#include <chrono>
#include <string>
#include <map>
#include <format>
#include <memory>

#include <nlohmann/json.hpp>

#include "../Services/Service.hpp"

struct DateTime
{
    std::chrono::year_month_day date{};
    std::chrono::hh_mm_ss<std::chrono::milliseconds> time{};

public:
    bool operator<(const DateTime &other) const
    {
        if (date == other.date)
            return time.to_duration() < other.time.to_duration();
        return date < other.date;
    }

    DateTime() = default;

    static std::string toLogString(const DateTime &dt)
    {
        // example    "[05Jul2025 08:33:37.817]"
        // indexs      012345678901234567890123
        std::string res = std::format("[{:02}{}{} {:02}:{:02}:{:02}.{:03}]",
            static_cast<unsigned>(dt.date.day()), toString(dt.date.month()), static_cast<int>(dt.date.year()), 
            dt.time.hours().count(), dt.time.minutes().count(), dt.time.seconds().count(), dt.time.subseconds().count());
        return res;
    }

    static DateTime fromLogString(const std::string &str)
    {
        // example    "[05Jul2025 08:33:37.817]"
        // indexs      012345678901234567890123
        if (str.size() < 24)
            return DateTime{};
        unsigned short month = parseMonth(str.substr(3, 3));
        if (month < 1 || month > 12)
            return DateTime{};
        unsigned short day = (str[1] - '0') * 10 + (str[2] - '0');
        unsigned short year = (str[6] - '0') * 1000 + (str[7] - '0') * 100 + (str[8] - '0') * 10 + (str[9] - '0');
        unsigned short hours = (str[11] - '0') * 10 + (str[12] - '0');
        unsigned short minutes = (str[14] - '0') * 10 + (str[15] - '0');
        unsigned short seconds = (str[17] - '0') * 10 + (str[18] - '0');
        unsigned short milliseconds = (str[20] - '0') * 100 + (str[21] - '0') * 10 + (str[22] - '0');

        if(!okTime(hours, minutes, seconds, milliseconds))
            return DateTime{};

        DateTime res;
        res.date = std::chrono::year_month_day(std::chrono::year{year}, std::chrono::month{month}, std::chrono::day{day});
        std::chrono::milliseconds total_ms =
                std::chrono::milliseconds(milliseconds) + 
                std::chrono::seconds(seconds) +
                std::chrono::minutes(minutes) +
                std::chrono::hours(hours);
        res.time = std::chrono::hh_mm_ss<std::chrono::milliseconds>(total_ms);
        if(!res.date.ok())
            return DateTime{};
        return res;
    }

private: // static

    using ConverterMapType = std::unique_ptr<const std::map<std::string, unsigned short>>;

    static ConverterMapType converterPtr_;

    static bool okTime(const unsigned short h, const unsigned short m, const unsigned short s, const unsigned short mil)
    {
        return (h < 24 && m < 60 && s < 60 && mil < 1000);
    }

    static unsigned short parseMonth(const std::string s)
    {
        if(converterPtr_ == nullptr)
            converterPtr_ = std::make_unique<const std::map<std::string, unsigned short>>(DateTime::parseJson(Service::config.MONTHS_PATH));
        const auto found = converterPtr_->find(s);
        return found != converterPtr_->end() ? found->second : 0;
    }

    static std::string toString(const std::chrono::month m)
    {
        if(converterPtr_ == nullptr)
            converterPtr_ = std::make_unique<const std::map<std::string, unsigned short>>(DateTime::parseJson(Service::config.MONTHS_PATH));
        for (const auto &[str, month] : *converterPtr_)
            if (month == static_cast<unsigned int>(m))
                return str;
        return "Non";
    }

    static std::map<std::string, unsigned short> parseJson(const std::string &fileName)
    {
        std::ifstream in(fileName);
        nlohmann::json j = nlohmann::json::parse(in);
        in.close();
        return j.get<std::map<std::string, unsigned short>>();
    }
};

DateTime::ConverterMapType DateTime::converterPtr_ = nullptr;