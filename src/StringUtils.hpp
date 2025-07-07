#pragma once

#include <list>
#include <string>

namespace StringUtils
{
    
    std::list<std::string> split(const std::string& s, const char c)
    {
        std::list<std::string> res;
        std::size_t last = 0;
        for(std::size_t ind = s.find(c); ind != std::string::npos;last = ind, ind = s.find(c, ind + 1))
            res.push_back(s.substr(last, ind - last));
        res.push_back(s.substr(last));
        return res;
    }


} // namespace StringUrilt
