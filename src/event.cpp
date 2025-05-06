#include "event.h"
#include "utils.h"
#include <sstream>
#include <iostream>

Event Event::parse(const std::string &line)
{
    std::istringstream iss(line);
    std::string time;
    int id;
    std::string body;
    iss >> time >> id;
    std::getline(iss >> std::ws, body);
    return Event{time, id, body};
}

std::string Event::toString() const
{
    std::ostringstream oss;
    oss << time << " " << id << " " << body;
    std::string result = oss.str();
    return result;
}