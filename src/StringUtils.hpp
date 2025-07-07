#pragma once

#include <list>
#include <string>
#include <format>

namespace StringUtils
{

    std::list<std::string> split(const std::string &s, const char c)
    {
        std::list<std::string> res;
        std::size_t last = 0;
        for (std::size_t ind = s.find(c); ind != std::string::npos; last = ind + 1, ind = s.find(c, last))
            res.push_back(s.substr(last, ind - last));
        res.push_back(s.substr(last));
        return res;
    }

    void replaceAll(std::string &str, const std::string &oldS, const std::string &newS)
    {
        if (oldS.empty())
            return;

        for (std::size_t pos = str.find(oldS); pos != std::string::npos; pos = str.find(oldS, pos + newS.length()))
            str.replace(pos, oldS.length(), newS);
    }

} // namespace StringUrilt
