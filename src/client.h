#pragma once
#include <string>

class Client
{
public:
    explicit Client(const std::string &name);
    void arrive(const std::string &time);
    void sit(int tableNumber, const std::string &time);
    void wait();
    void leave();
    bool isInClub() const;
    bool isSitting() const;
    int getTable() const;

private:
    std::string name_;
    bool inClub_;
    bool sitting_;
    int tableNumber_;
    std::string arrivalTime_;
    std::string sessionStart_;
};