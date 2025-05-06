#include "utils.h"
#include <cstdio>
#include <stdexcept>
#include <iostream>

namespace Time
{
    bool isValid(const std::string &time)
    {
        for (size_t i = 0; i < time.length(); ++i)
        {
            if (i == 2)
                continue;
        }
        int hours = std::stoi(time.substr(0, 2));
        int minutes = std::stoi(time.substr(3, 2));
        bool valid = (hours >= 0 && hours <= 23) && (minutes >= 0 && minutes <= 59);
        return valid;
    }

    int toMinutes(const std::string &time)
    {
        size_t colonPos = time.find(':');

        int hours = std::stoi(time.substr(0, colonPos));
        int minutes = std::stoi(time.substr(colonPos + 1));
        int totalMinutes = hours * 60 + minutes;
        return totalMinutes;
    }

    std::string toTime(int minutes)
    {
        int hours = minutes / 60;
        int mins = minutes % 60;
        char buffer[8];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", hours % 24, mins);
        return std::string(buffer);
    }

    int diffMinutes(const std::string &start, const std::string &end)
    {
        int startMinutes = toMinutes(start);
        int endMinutes = toMinutes(end);
        if (endMinutes >= startMinutes)
        {
            return endMinutes - startMinutes;
        }
        else
        {
            return (24 * 60 - startMinutes) + endMinutes;
        }
    }

    int ceiltoHour(int minutes)
    {
        int hours = (minutes + 59) / 60;
        return hours;
    }
}