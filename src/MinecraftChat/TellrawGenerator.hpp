#pragma once

#include <vector>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "../StringUtils.hpp"

class TellrawGenerator
{
public:

    std::string paramsToTellraw(const std::vector<nlohmann::json>& params) const
    {
        std::string res = "/tellraw @a [";
        for(const auto& i : params)
            res += i.dump() + ',';
        res.pop_back();
        res += ']';
        return res;
    }

    std::string baseMessage(const std::string& playerName, const std::string& messangeText) const
    {
        std::string text = preprocess(messangeText);

        std::vector<nlohmann::json> params;
        params.push_back(getBaseText('<' + playerName + '>' + ' ', "aqua"));
        fillUrls(params, messangeText);

        return paramsToTellraw(params);
    }

    std::string mediaMessage(const std::string& playerName)
    {
        std::vector<nlohmann::json> params;
        params.push_back(getBaseText('<' + playerName + '>' + ' ', "aqua"));
        params.push_back(getBaseText("media file", "blue"));

        return paramsToTellraw(params);
    }
    

private:

    std::string preprocess(const std::string& str) const
    {
        constexpr static const char symbols[] = "[]{}\n\t\"\'";
        constexpr static const std::size_t size = sizeof(symbols) / sizeof(char);

        std::string res;
        res.reserve(str.size());
        bool replaced = false;
        for(const auto& c : str)
        {
            replaced = false;
            for(const auto& specC : symbols)
                if(specC == c)
                {
                    replaced = true;
                    res += '\\' + c;
                    break;
                }
            if(!replaced)
                res += c;
        }
        return res;
    }

    void preprocessString(std::string& str)
    {
        //Скбоки
        StringUtils::replaceAll(str, "[", "\\[");
        StringUtils::replaceAll(str, "]", "\\]");
        StringUtils::replaceAll(str, "{", "\\{");
        StringUtils::replaceAll(str, "}", "\\}");
        //Спец символы
        StringUtils::replaceAll(str, "\n", "\\n");
        StringUtils::replaceAll(str, "\t", " ");
        //Ковычки
        StringUtils::replaceAll(str, "\"", "\\\"");
        StringUtils::replaceAll(str, "\'", "\\\'");
    }


    nlohmann::json getBaseText(const std::string_view text, const std::string& color = "white") const
    {
        nlohmann::json res;
        res["text"] = text;
        res["color"] = color;
        return res;
    }
    nlohmann::json getLink(const std::string_view url) const
    {
        nlohmann::json event;
        event["action"] = "open_url";
        event["value"] = url;

        nlohmann::json res;
        res["text"] = url;
        res["color"] = "blue";
        res["clickEvent"] = event;

        return res;
    }

    void fillUrls(std::vector<nlohmann::json>& params, const std::string_view str) const
    {
        auto findLink = [&](const std::size_t i)
        {
            return (std::min)(str.find("https://", i), str.find("http://", i));
        };

        size_t lastPos = 0, ind = 0;
        for(ind = findLink(0); ind < str.size(); lastPos = ind, ind = findLink(lastPos + 1))
        {
            if(lastPos != ind)
            {
                params.push_back(getBaseText(str.substr(lastPos, ind - lastPos)));
                lastPos = ind;
                ind = (std::min)(str.find(' ', ind), str.size());
            }
            params.push_back(getLink(str.substr(lastPos, ind - lastPos)));
        }
        params.push_back(getBaseText(str.substr(lastPos)));
    }



};