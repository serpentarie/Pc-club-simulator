#pragma once
#include <string>

struct Event
{
    std::string time;
    int id;
    std::string body;

    static Event parse(const std::string &line);
    std::string toString() const;
};