#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <map>
#include <iostream>
#include <utility>
#include <chrono>
#include <format>

enum class LogLevel : unsigned char
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Critical
};

class ServerLogger
{
public:
    ServerLogger() = default;
    ServerLogger(const std::string &fileName, const bool writeToConsole = false) : writeToConsole_{writeToConsole}
    {
        openFile(fileName);
    }
    ~ServerLogger()
    {
        closeFile();
    }

    bool isEntryToConsole() const
    {
        return writeToConsole_;
    }
    void setEntryToConsole(const bool writeToConsole)
    {
        writeToConsole_ = writeToConsole;
    }

    void closeFile()
    {
        log("---  Log session ended  ---", LogLevel::Info);
        logFile_.close();
    }
    bool openFile(const std::string &fileName)
    {
        if (isOpen())
            closeFile();
        logFile_.open(fileName, std::ios::app);
        log("--- Log session started ---", LogLevel::Info);
        return logFile_.is_open();
    }
    bool isOpen() const
    {
        return logFile_.is_open();
    }

    void log(std::string message, LogLevel level)
    {
        std::string time = getCurentTime();

        const auto [strName, color] = toString(level);

        std::string resultMessage = time + ' ' + strName + ' ' + message + '\n';

        if (logFile_.is_open())
        {
            logFile_ << resultMessage;
            logFile_.flush();
        }
        if (writeToConsole_)
        {
            setColor(color);
            std::cerr << resultMessage;
            setColor(Color::Default);
        }
    }

private:

    std::ofstream logFile_;
    bool writeToConsole_ = false;

private:

    #pragma region Static

    enum class Color : unsigned char
    {
        Black = 30, // DEBUG
        Red,        // ERROR
        Green,      // INFO
        Yellow,     // WARNING
        Blue,
        Purple, // CRITICAL
        Cyan,
        White,
        Default = 39 // DEBUG
    };

    static void setColor(const Color c)
    {
        std::cerr << '\x1b' << '[' << static_cast<int>(c) << 'm';
    }

    static std::pair<std::string, Color> toString(const LogLevel level)
    {
        // clang-format off
        switch (level)
        {
            case LogLevel::Critical:  return {"[  FATAL  ]", Color::Purple  };
            case LogLevel::Debug:     return {"[  DEBUG  ]", Color::Default };
            case LogLevel::Error:     return {"[  ERROR  ]", Color::Red     };
            case LogLevel::Info:      return {"[   INF   ]", Color::Green   };
            case LogLevel::Warning:   return {"[ WARNING ]", Color::Yellow  };
            default:                  return {"[ UNKNOWN ]", Color::Default };
        }
        // clang-format on

        // // clang-format off
        // static const std::map<LogLevel, std::pair<std::string, Color>> converter =
        // {
        //     {LogLevel::Critical,  {"[ CRITICAL ]", Color::Purple  }},
        //     {LogLevel::Debug,     {"[ DEBUG    ]", Color::Default }},
        //     {LogLevel::Error,     {"[ ERROR    ]", Color::Red     }},
        //     {LogLevel::Info,      {"[ INFO     ]", Color::Green   }},
        //     {LogLevel::Warning,   {"[ WARNING  ]", Color::Yellow  }}
        // };
        // // clang-format on
        // std::cout << converter.empty() << '\n';
        // return converter.find(level)->second;
    }

    static std::string getCurentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;
        return std::format("({:%d-%m-%Y %X}:{:03d})", now, ms.count());
    }

    #pragma endregion

};
