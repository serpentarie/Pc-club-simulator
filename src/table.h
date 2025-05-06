#pragma once
#include <string>

class Table
{
public:
    explicit Table(int number);
    bool isOccupied() const;
    void occupy(const std::string &client, const std::string &startTime);
    void free(const std::string &endTime, int costPerHour);
    int getNumber() const;
    int getRevenue() const;
    std::string getOccupiedTime() const;

private:
    int number_;
    bool occupied_;
    std::string client_;
    std::string sessionStart_;
    int totalOccupiedMinutes_;
    int revenue_;
};