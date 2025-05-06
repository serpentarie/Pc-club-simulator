#pragma once
#include <vector>
#include <map>
#include <queue>
#include "event.h"
#include "table.h"
#include "client.h"

struct ClubConfig
{
    int numTables;
    std::string startTime;
    std::string endTime;
    int costPerHour;
};

class Club
{
public:
    Club(const ClubConfig &config, const std::vector<Event> &events);
    void process();
    void printResults() const;
    const std::vector<Event> &getOutputEvents() const { return outputEvents_; }
    const std::vector<Table> &getTables() const { return tables_; }

private:
    ClubConfig config_;
    std::vector<Table> tables_;
    std::map<std::string, Client> clients_;
    std::queue<std::string> waitingQueue_;
    std::vector<Event> outputEvents_;

    void handleClientArrived(const Event &event);
    void handleClientSat(const Event &event);
    void handleClientWaiting(const Event &event);
    void handleClientLeft(const Event &event);
    void endDay();
};
