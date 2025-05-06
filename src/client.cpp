#include "client.h"
#include "utils.h"

Client::Client(const std::string &name)
    : name_(name),
      inClub_(false),
      sitting_(false),
      tableNumber_(0),
      arrivalTime_(""),
      sessionStart_("") {}

void Client::arrive(const std::string &time)
{
    inClub_ = true;
    arrivalTime_ = time;
}

void Client::sit(int tableNumber, const std::string &time)
{
    sitting_ = true;
    tableNumber_ = tableNumber;
    sessionStart_ = time;
}

void Client::wait()
{
    sitting_ = false;
    tableNumber_ = 0;
    sessionStart_.clear();
}

void Client::leave()
{
    inClub_ = false;
    sitting_ = false;
    tableNumber_ = 0;
    arrivalTime_.clear();
    sessionStart_.clear();
}

bool Client::isInClub() const
{
    return inClub_;
}

bool Client::isSitting() const
{
    return sitting_;
}

int Client::getTable() const
{
    return tableNumber_;
}