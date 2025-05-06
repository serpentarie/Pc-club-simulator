#include "club.h"
#include "utils.h"
#include <algorithm>
#include <sstream>
#include <iostream>

Club::Club(const ClubConfig &config, const std::vector<Event> &events)
    : config_(config), outputEvents_(events)
{
    for (int i = 1; i <= config.numTables; ++i)
    {
        tables_.emplace_back(i);
    }
}

void Club::process()
{
    std::vector<Event> eventsToProcess = outputEvents_;
    outputEvents_.clear();
    for (const auto &event : eventsToProcess)
    {
        switch (event.id)
        {
        case 1:
            handleClientArrived(event);
            break;
        case 2:
            handleClientSat(event);
            break;
        case 3:
            handleClientWaiting(event);
            break;
        case 4:
            handleClientLeft(event);
            break;
        default:
            outputEvents_.push_back(Event{event.time, 13, "error"});
        }
    }
    endDay();
}

void Club::handleClientArrived(const Event &event)
{
    try
    {
        outputEvents_.push_back(event);
    }
    catch (const std::exception &e)
    {
        return;
    }

    bool eventValid = Time::isValid(event.time);
    bool startValid = Time::isValid(config_.startTime);
    bool endValid = Time::isValid(config_.endTime);

    if (!eventValid || !startValid || !endValid)
    {
        return;
    }

    try
    {
        int eventMinutes = Time::toMinutes(event.time);
        int startMinutes = Time::toMinutes(config_.startTime);
        int endMinutes = Time::toMinutes(config_.endTime);

        if (eventMinutes < startMinutes || eventMinutes >= endMinutes)
        {
            outputEvents_.push_back(Event{event.time, 13, "NotOpenYet"});
            return;
        }

        if (clients_.count(event.body))
        {
            outputEvents_.push_back(Event{event.time, 13, "YouShallNotPass"});
            return;
        }

        clients_.emplace(event.body, Client(event.body));
        clients_.at(event.body).arrive(event.time);
    }
    catch (const std::exception &e)
    {
    }
}

void Club::handleClientSat(const Event &event)
{
    outputEvents_.push_back(event);

    std::istringstream iss(event.body);
    std::string clientName;
    int tableNumber;
    if (!(iss >> clientName >> tableNumber))
    {
        return;
    }

    if (!clients_.count(clientName))
    {
        outputEvents_.push_back(Event{event.time, 13, "ClientUnknown"});
        return;
    }

    if (tableNumber < 1 || tableNumber > config_.numTables)
    {
        outputEvents_.push_back(Event{event.time, 13, "PlaceIsBusy"});
        return;
    }

    if (clients_.at(clientName).isSitting())
    {
        int oldTable = clients_.at(clientName).getTable() - 1;
        tables_[oldTable].free(event.time, config_.costPerHour);
    }

    if (tables_[tableNumber - 1].isOccupied())
    {
        outputEvents_.push_back(Event{event.time, 13, "PlaceIsBusy"});
        return;
    }

    tables_[tableNumber - 1].occupy(clientName, event.time);
    clients_.at(clientName).sit(tableNumber, event.time);
}

void Club::handleClientWaiting(const Event &event)
{
    outputEvents_.push_back(event);

    const std::string &clientName = event.body;

    if (!clients_.count(clientName))
    {
        outputEvents_.push_back(Event{event.time, 13, "ClientUnknown"});
        return;
    }

    bool hasFreeTables = false;
    for (const auto &table : tables_)
    {
        if (!table.isOccupied())
        {
            hasFreeTables = true;
            break;
        }
    }

    if (hasFreeTables)
    {
        outputEvents_.push_back(Event{event.time, 13, "ICanWaitNoLonger!"});
        return;
    }

    if (waitingQueue_.size() >= config_.numTables)
    {
        clients_.at(clientName).leave();
        clients_.erase(clientName);
        outputEvents_.push_back(Event{event.time, 11, clientName});
        return;
    }

    waitingQueue_.push(clientName);
    clients_.at(clientName).wait();
}

void Club::handleClientLeft(const Event &event)
{
    outputEvents_.push_back(event);

    const std::string &clientName = event.body;

    if (!clients_.count(clientName))
    {
        outputEvents_.push_back(Event{event.time, 13, "ClientUnknown"});
        return;
    }

    if (clients_.at(clientName).isSitting())
    {
        int tableNumber = clients_.at(clientName).getTable() - 1;
        tables_[tableNumber].free(event.time, config_.costPerHour);

        if (!waitingQueue_.empty())
        {
            std::string nextClient = waitingQueue_.front();
            waitingQueue_.pop();
            tables_[tableNumber].occupy(nextClient, event.time);
            clients_.at(nextClient).sit(tableNumber + 1, event.time);
            outputEvents_.push_back(Event{event.time, 12, nextClient + " " + std::to_string(tableNumber + 1)});
        }
    }

    clients_.at(clientName).leave();
    clients_.erase(clientName);
}

void Club::endDay()
{
    std::vector<std::string> remainingClients;
    for (const auto &[name, _] : clients_)
    {
        remainingClients.push_back(name);
    }
    std::sort(remainingClients.begin(), remainingClients.end());

    for (const auto &clientName : remainingClients)
    {
        if (clients_.count(clientName) && clients_.at(clientName).isSitting())
        {
            int tableNumber = clients_.at(clientName).getTable() - 1;
            tables_[tableNumber].free(config_.endTime, config_.costPerHour);
        }
        outputEvents_.push_back(Event{config_.endTime, 11, clientName});
        clients_.erase(clientName);
    }
}

void Club::printResults() const
{
    std::cout << config_.startTime << "\n";
    for (const auto &event : outputEvents_)
    {
        std::cout << event.time << " " << event.id << " " << event.body << "\n";
    }
    std::cout << config_.endTime << "\n";
    for (const auto &table : tables_)
    {
        std::cout << table.getNumber() << " " << table.getRevenue() << " "
                  << table.getOccupiedTime() << "\n";
    }
}