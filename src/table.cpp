#include "table.h"
#include "utils.h"
#include <stdexcept>

Table::Table(int number)
    : number_(number),
      occupied_(false),
      client_(""),
      sessionStart_(""),
      totalOccupiedMinutes_(0),
      revenue_(0) {}

bool Table::isOccupied() const
{
    return occupied_;
}

void Table::occupy(const std::string &client, const std::string &startTime)
{
    occupied_ = true;
    client_ = client;
    sessionStart_ = startTime;
}

void Table::free(const std::string &endTime, int costPerHour)
{
    int duration = Time::diffMinutes(sessionStart_, endTime);
    int hours = Time::ceiltoHour(duration);
    int cost = hours * costPerHour;

    totalOccupiedMinutes_ += duration;
    revenue_ += cost;

    occupied_ = false;
    client_.clear();
    sessionStart_.clear();
}

int Table::getNumber() const
{
    return number_;
}

int Table::getRevenue() const
{
    return revenue_;
}

std::string Table::getOccupiedTime() const
{
    return Time::toTime(totalOccupiedMinutes_);
}