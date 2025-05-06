#include "parser.h"
#include "utils.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <algorithm>

bool InputParser::parse(std::ifstream &file)
{
    std::string line;
    int lineNumber = 0;

    try
    {
        if (!std::getline(file, line))
        {
            return false;
        }
        lineNumber++;

        config_.numTables = std::stoi(line);
        if (config_.numTables <= 0)
        {
            return false;
        }

        if (!std::getline(file, line))
        {
            return false;
        }
        lineNumber++;

        std::istringstream timeStream(line);
        std::string startTime, endTime;
        if (!(timeStream >> startTime >> endTime))
        {
            return false;
        }

        if (!Time::isValid(startTime) || !Time::isValid(endTime))
        {
            return false;
        }
        config_.startTime = startTime;
        config_.endTime = endTime;

        if (!std::getline(file, line))
        {
            return false;
        }
        lineNumber++;

        config_.costPerHour = std::stoi(line);
        if (config_.costPerHour <= 0)
        {
            return false;
        }

        while (std::getline(file, line))
        {
            lineNumber++;
            if (line.empty())
                continue;

            line.erase(std::remove_if(line.begin(), line.end(), [](char c)
                                      { return !std::isprint(c); }),
                       line.end());
            try
            {
                events_.push_back(Event::parse(line));
            }
            catch (const std::exception &e)
            {
                return false;
            }
        }

        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

ClubConfig InputParser::getConfig() const
{
    return config_;
}

std::vector<Event> InputParser::getEvents() const
{
    return events_;
}

std::string InputParser::getErrorLine() const
{
    return errorLine_;
}