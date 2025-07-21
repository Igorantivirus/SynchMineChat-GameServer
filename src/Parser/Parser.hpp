#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <limits>
#include <queue>

#include "../Services/Service.hpp"

#include "LogMessage.hpp"

class MinecraftLogParser
{
public:
    MinecraftLogParser()
    {
        outLastPosFile((std::numeric_limits<std::streamsize>::max)());
        // checkLastPosFile();
    }

    void parse(std::queue<LogMessage> &queue)
    {
        std::ifstream logFile;
        openLogFile(logFile);
        
        std::string line;
        LogMessage msg;
        while (getline(logFile, line))
            if(msg.parseFromString(line))
                queue.push(msg);

        // if(queue.size() > 1)
        // {
        //     std::cout << "A\n";
        // }

        logFile.close();
    }

private:

    void openLogFile(std::ifstream& logFile)
    {
        logFile.open(Service::config.MINECRAFT_LOG_FILE, std::ios::in | std::ios::binary);
        if (!logFile.is_open())
        {
            Service::log.log("Unable to open log file: " + Service::config.MINECRAFT_LOG_FILE, LogLevel::Error);
            return;
        }
        std::streamsize fileSize = getFileSize(logFile);
        std::streamsize lastSize = getLastPos();
        
        if(lastSize < fileSize)
            logFile.seekg(lastSize, std::ios_base::beg);
        else
            logFile.seekg(fileSize, std::ios_base::beg);

        outLastPosFile(fileSize);
    }

    // void checkLastPosFile()
    // {
    //     if(std::filesystem::exists(Service::config.LAST_POS_FILE))
    //         return;
    //     std::ofstream out(Service::config.LAST_POS_FILE);
    //     out << std::streamsize(0);
    //     out.close();
    // }
    
    void outLastPosFile(const std::streamsize pos)
    {
        std::ofstream out(Service::config.LAST_POS_FILE);
        out << pos;
        out.close();
    }
    
    std::streamsize getLastPos()
    {
        std::ifstream in{Service::config.LAST_POS_FILE};
        if(!in.is_open())
            return (std::numeric_limits<std::streamsize>::max)();
        std::streamsize res;
        in >> res;
        in.close();
        return res;
    }
    std::streamsize getFileSize(std::ifstream& logFile)
    {
        logFile.ignore((std::numeric_limits<std::streamsize>::max)());
        std::streamsize fileSize = logFile.gcount();
        logFile.clear();
        return fileSize;
    }

    //Код Коли, оставил, чтоб не потерять
    // void old()
    // {
    //     using namespace std;
    //     static streamsize latestLength = 0;
    //     static bool firstRun = true;
    //     if (firstRun)
    //     {
    //         firstRun = false;
    //         ifstream ifs(config.latest_length_path);
    //         if (ifs.is_open())
    //         {
    //             ifs >> latestLength;
    //             ifs.close();
    //         }
    //     }
    //     ifstream logFile(logFilePath, ios::in | ios::binary);
    //     if (!logFile.is_open())
    //     {
    //         cerr << "Error: Unable to open log file: " << logFilePath << endl;
    //         return;
    //     }
    //     logFile.ignore((numeric_limits<streamsize>::max)());
    //     streamsize length = logFile.gcount();
    //     logFile.clear();
    //     logFile.seekg(length < latestLength ? streamsize(0) : latestLength, ios_base::beg);
    //     latestLength = length;
    //     ofstream ofs(config.latest_length_path);
    //     if (ofs.is_open())
    //     {
    //         ofs << latestLength;
    //         ofs.close();
    //     }
    //     string line;
    //     while (getline(logFile, line))
    //     {
    //         if (line.find("[Not Secure]") != string::npos)
    //         {
    //             queue.push_back((line.substr(line.find("[Not Secure]") + 13)));
    //         }
    //     }
    //     logFile.close();
    // }
};
