#include <string>

namespace Time
{
    bool isValid(const std::string &time);
    int toMinutes(const std::string &time);
    std::string toTime(int minutes);
    int diffMinutes(const std::string &start, const std::string &end);
    int ceiltoHour(int minutes);
}