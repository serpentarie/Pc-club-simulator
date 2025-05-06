#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "event.h"
#include "club.h"

class InputParser
{
public:
    bool parse(std::ifstream &file);
    ClubConfig getConfig() const;
    std::vector<Event> getEvents() const;
    std::string getErrorLine() const;

private:
    ClubConfig config_;
    std::vector<Event> events_;
    std::string errorLine_;
};